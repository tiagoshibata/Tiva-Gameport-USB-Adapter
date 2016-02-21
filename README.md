# Tiva Launchpad based Gameport to USB adapter
---
Pinout:
```

      +3.3V 5V                    BLed => PF2 GND
        PB5 GND                   GLed => PF3 PB2
        PB0 PD0/PB6 <= X1 in PD0          PB3 PE0
        PB1 PD1/PB7 <= X2 in PD1          PC4 PF0 <= SW2
  B4 => PE4 PD2 <= Y1                     PC5 RST
        PE5 PD3 <= Y2                     PC6 PB7/PD1
        PB4 PE1 <= B1                     PC7 PB6/PD0
        PA5 PE2 <= B2                     PD6 PA4
        PA6 PE3 <= B3                     PD7 PA3
        PA7 PF1 <= RLed            SW1 => PF4 PA2

USB pins to Tiva:
        | VCC => VBUS
        | D-  => PD4
    USB | D+  => PD5
        | NC
        | GND => GND
```

Axis input pins are read using a resistive voltage divider. A ~10kOhm resistor
should be placed between analog inputs and GND.

Button inputs use Tiva's internal pull-up resistors.

Work in progress, lacks one axis and calibration.
