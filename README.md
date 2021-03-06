# ServoTester

Code for an Adafruit Feather M0 and 128x32 Featherwing OLED display.

![Image of ServoTester](https://github.com/amcchord/ServoTester/blob/master/doc/ServoTester.jpg)

## Hardware Config

Pin 10 is designated as the servo output

## Modes

### Manual Mode

Manually set the servo angle using buttons A and B

### Sweep Mode

Servo sweeps up and down between 0 and 180 degrees. Buttons A + B set speed

### Ramp Mode

Servo ramps from 0 and 180 degrees. Buttons A + B set speed

### Random Mode

Servo randomly moves to angles from 0 and 180 degrees. Buttons A + B set speed
This mode is good for testing gear trains to ensure they don't fail under jerky movement.

### Min / max

The bounds for servo output can bet set from the m/M menu.

## Features

- The last servo output that was set at the main menu persists across reboots.
- Easy to wire so servo's can be powered by USB or have the tester powered by the BEC
