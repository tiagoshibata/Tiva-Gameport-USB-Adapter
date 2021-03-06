#pragma once

#define GPIO_JOYSTICK_AXIS_BASE		GPIO_PORTD_AHB_BASE
#define GPIO_JOYSTICK_AXIS_X1		GPIO_PIN_0
#define GPIO_JOYSTICK_AXIS_Y1		GPIO_PIN_2
#define GPIO_JOYSTICK_AXIS_X2		GPIO_PIN_1
#define GPIO_JOYSTICK_AXIS_Y2		GPIO_PIN_3

#define GPIO_JOYSTICK_BUTTON_BASE	GPIO_PORTE_BASE
#define GPIO_JOYSTICK_BUTTON_1		GPIO_PIN_1
#define GPIO_JOYSTICK_BUTTON_2		GPIO_PIN_2
#define GPIO_JOYSTICK_BUTTON_3		GPIO_PIN_3
#define GPIO_JOYSTICK_BUTTON_4		GPIO_PIN_4
#define GPIO_JOYSTICK_BUTTON_ALL	(GPIO_JOYSTICK_BUTTON_1 | \
	GPIO_JOYSTICK_BUTTON_2 | GPIO_JOYSTICK_BUTTON_3 | GPIO_JOYSTICK_BUTTON_4)

#define GPIO_BOARD_SWITCHES_BASE	GPIO_PORTF_BASE
#define GPIO_BOARD_SW1				GPIO_PIN_4
#define GPIO_BOARD_SW2				GPIO_PIN_0
#define GPIO_BOARD_SW_ALL			(GPIO_BOARD_SW1 | GPIO_BOARD_SW2)

void input_init();
uint32_t input_buttons_read();
uint32_t input_enter_calibration();
void input_axis_calibrate();
void input_axis_read();
