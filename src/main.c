#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <inc/hw_sysctl.h>

#include "input.h"
#include "leds.h"
#include "usb.h"

#define Convert8Bit(ui32Value)  ((int8_t)((0x7ff - ui32Value) >> 4))

int main() {
	// Use 50MHz
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);		// Analog axis
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);	// USB, analog inputs
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);	// buttons
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);	// board LEDs and switches
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);		// PWM Module 1

	leds_init();
	usb_init();
	input_init();

	// leds_b(4000);
	// input_axis_calibrate();
	// leds_b(0);

	for (;;) {
		uint32_t buttons = input_buttons_read();
		if (buttons != usb_report.buttons) {
			uint32_t new_buttons;
			for (;;) {
				ROM_SysCtlDelay(1000);
				new_buttons = input_buttons_read();
				if (new_buttons == buttons)
					break;
				buttons = new_buttons;
			}
			usb_report.buttons = buttons;

			usb_schedule_report();
		}

		/// @todo Axis calibration
		// if (ROM_ADCIntStatus(ADC0_BASE, 0, false) != 0) {
		// 	uint8_t axis[4];
		// 	// input_axis_read(axis);
		// 	usb_report.x1 = axis[0];
		// 	usb_report.y1 = axis[1];
		// 	usb_report.x2 = axis[2];
		// 	usb_schedule_report();
		// }

		// if (input_enter_calibration()) {
		// 	leds_b(4000);
		// 	input_axis_calibrate();
		// 	leds_b(0);
		// }
	}
}
