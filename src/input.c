/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Tiago Koji Castro Shibata

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#include "input.h"

#include <driverlib/adc.h>
#include <driverlib/gpio.h>
#include <driverlib/rom.h>
#include <inc/hw_gpio.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>

static uint32_t axis_zero[4];
static uint32_t axis_max[4];
static uint32_t axis_min[4];

#define AXIS_COUNT	((int) (sizeof(axis_max) / sizeof(*axis_max)))

void input_init() {
	// Analog inputs
	ROM_GPIOPinTypeADC(GPIO_JOYSTICK_AXIS_BASE, GPIO_PIN_0);
	ROM_GPIOPinTypeADC(GPIO_JOYSTICK_AXIS_BASE, GPIO_PIN_1);
	ROM_GPIOPinTypeADC(GPIO_JOYSTICK_AXIS_BASE, GPIO_PIN_2);
	ROM_GPIOPinTypeADC(GPIO_JOYSTICK_AXIS_BASE, GPIO_PIN_3);

	// Configure clock and oversampling
	ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_SRC_PIOSC | ADC_CLOCK_RATE_FULL, 1);
	ROM_ADCHardwareOversampleConfigure(ADC0_BASE, 32);

	// Sequence configuration. Channels 7-4 correspond to PD0-PD3
	ROM_ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_ALWAYS, 0);
	ROM_ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH7);
	ROM_ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH5);
	ROM_ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_CH6);
	ROM_ADCSequenceStepConfigure(ADC0_BASE, 0, 3, ADC_CTL_CH4 | ADC_CTL_IE | ADC_CTL_END);

	ROM_ADCSequenceEnable(ADC0_BASE, 0);

	// Joystick input buttons
	ROM_GPIOPinTypeGPIOInput(GPIO_JOYSTICK_BUTTON_BASE, GPIO_JOYSTICK_BUTTON_1);
	ROM_GPIOPinTypeGPIOInput(GPIO_JOYSTICK_BUTTON_BASE, GPIO_JOYSTICK_BUTTON_2);
	ROM_GPIOPinTypeGPIOInput(GPIO_JOYSTICK_BUTTON_BASE, GPIO_JOYSTICK_BUTTON_3);
	ROM_GPIOPinTypeGPIOInput(GPIO_JOYSTICK_BUTTON_BASE, GPIO_JOYSTICK_BUTTON_4);
	// Board input buttons
	// Unlock PF0 (must be unlocked to use as input because it is a NMI pin)
	HWREG(GPIO_BOARD_SWITCHES_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_BOARD_SWITCHES_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_BOARD_SWITCHES_BASE + GPIO_O_LOCK) = 0;
	ROM_GPIOPinTypeGPIOInput(GPIO_BOARD_SWITCHES_BASE, GPIO_BOARD_SW1);
	ROM_GPIOPinTypeGPIOInput(GPIO_BOARD_SWITCHES_BASE, GPIO_BOARD_SW2);

	// Pull-up
	ROM_GPIOPadConfigSet(GPIO_JOYSTICK_BUTTON_BASE, GPIO_JOYSTICK_BUTTON_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	ROM_GPIOPadConfigSet(GPIO_JOYSTICK_BUTTON_BASE, GPIO_JOYSTICK_BUTTON_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	ROM_GPIOPadConfigSet(GPIO_JOYSTICK_BUTTON_BASE, GPIO_JOYSTICK_BUTTON_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	ROM_GPIOPadConfigSet(GPIO_JOYSTICK_BUTTON_BASE, GPIO_JOYSTICK_BUTTON_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	ROM_GPIOPadConfigSet(GPIO_BOARD_SWITCHES_BASE, GPIO_BOARD_SW1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	ROM_GPIOPadConfigSet(GPIO_BOARD_SWITCHES_BASE, GPIO_BOARD_SW2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

uint32_t input_buttons_read() {
	uint32_t buttons;

	buttons = (ROM_GPIOPinRead(GPIO_JOYSTICK_BUTTON_BASE, GPIO_JOYSTICK_BUTTON_ALL) >> 1) ^ 0x0f;
	// Check if hat button pressed (B1 and B2)
	if ((buttons & 0x3) == 0x3)
		// B3 and B4 select hat position
		return 1 << (4 + (buttons >> 2));
	return buttons;
}

uint32_t input_enter_calibration() {
	// Enter calibration if both board switches are pressed
	return !(ROM_GPIOPinRead(GPIO_BOARD_SWITCHES_BASE, GPIO_BOARD_SW_ALL) & GPIO_BOARD_SW_ALL);
}

void input_axis_calibrate() {
	// Discard a reading
	ROM_ADCSequenceDataGet(ADC0_BASE, 0, axis_zero);
	ROM_ADCIntClear(ADC0_BASE, 0);
	while (!ROM_ADCIntStatus(ADC0_BASE, 0, false)) ;

	ROM_ADCIntClear(ADC0_BASE, 0);
	ROM_ADCSequenceDataGet(ADC0_BASE, 0, axis_zero);

	for (int i = 0; i < AXIS_COUNT; i++) {
		axis_min[i] = 4294967295;
		axis_max[i] = 0;
	}

	while (!(input_buttons_read() & 1)) {
		uint32_t axis[4];

		while (!ROM_ADCIntStatus(ADC0_BASE, 0, false)) ;
		ROM_ADCIntClear(ADC0_BASE, 0);
		ROM_ADCSequenceDataGet(ADC0_BASE, 0, axis);

		for (int i = 0; i < AXIS_COUNT; i++) {
			if (axis[i] < axis_min[i])
				axis_min[i] = axis[i];
			if (axis[i] > axis_max[i])
				axis_max[i] = axis[i];
		}
	}
}

void input_axis_read(uint8_t *converted_axis) {
	uint32_t axis[4];
	float axis_float;
	ROM_ADCIntClear(ADC0_BASE, 0);
	ROM_ADCSequenceDataGet(ADC0_BASE, 0, axis);

	for (int i = 0; i < AXIS_COUNT; i++) {
		axis_float = ((float) axis[i] - axis_min[i]) / (axis_max[i] - axis_min[i]) - .5;
		if (axis_float > 1.)
			axis_float = 1.;
		if (axis_float < -1.)
			axis_float = -1.;
		converted_axis[i] = (int8_t) (axis_float * 127);
	}
}
