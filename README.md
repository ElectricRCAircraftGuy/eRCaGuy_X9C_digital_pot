# eRCaGuy_X9C_digital_pot

Arduino library/driver for the Renesas X9C102 (1k), X9C103 (10k), X9C503 (50k), and X9C104 (100k) family of digital potentiometers.

Easily drive these 100-step digital potentiometers to any absolute or relative position or voltage.

By Gabriel Staples  
https://gabrielstaples.com  
www.ElectricRCAircraftGuy.com  


# Library Notes

This library works on any and all Arduino or Arduino-compatible boards. Set these pins passed to the `X9C_digital_pot` class constructor to any `digitalWrite()`-capable pin number for your board as you see fit. 

**For the ATMega168/ATmega328-based boards**, such as the **Uno/Nano/Mini**, using pins `0` and `1`, however, is generally NOT recommended, as they are for serial communication to upload your program. Therefore, I recommend Digital pins `2` through `13`, inclusive, or Analog pins `A0` through `A5`, inclusive. NB: If your ATmega168/328-based board has an `A6` or `A7` pin, these pins do NOT support `digitalWrite()`/`digitalRead()` like other analog pins! Rather, `A6` and `A7` can do `analogRead()` only.


# Status

Library is done. It is complete and thorough, and it compiles and runs on an Arduino. 

However, it is untested with **actual X9C103 digital potentiometer hardware**, as I'm still waiting on it to arrive in the mail. 

Examples are written. Please take a look, try them out, and see if they work!


# License
MIT


# Screenshots

[![](extras/pics/advanced_test_serial_monitor_screenshot.jpg)](extras/pics/advanced_test_serial_monitor_screenshot.jpg)


# Notes to self
- Reminder to follow this specification, folder layout, and to have a library.properties file or else Arduino won't accept and build your library! https://arduino.github.io/arduino-cli/latest/library-specification/ /////////////////////

WRITE BLOG POST ON ARDUINO LIBRARY FOLDER LAYOUT, THE ERROR MESSAGE, ETC.


_This library layout and structure conforms with the [Arduino Library specification 1.5 library format (rev. 2.2)](https://arduino.github.io/arduino-cli/latest/library-specification/)._


# TODO (delete when done--newest on bottom)
1. Write blog posts:
    1. How to make an Arduino library (focus on the mechanics and library specification, not really the code itself).
    1. Introducing this library briefly.
    1. Also do a [PPM library](https://github.com/ElectricRCAircraftGuy/eRCaGuy_PPM_Writer) intro while at it too.
