/*
Gabriel Staples

Basic demo of this library.

Circuit:
X9C IC Pin  Arduino Pin   Description
----------  -----------   -----------
Inc         See code      Increment
U/D         See code      Up/Down selection
VH          5V            Potentiometer High side input
Vss         GND           IC GND
Vcc         5V            IC Power
CS          See code      Chip Select
VL          GND           Potentiometer Low side (GND)
Vw          -             Wiper output---this is your output voltage signal to control
                          whatever you want!

*/

#include <eRCaGuy_X9C_digital_pot.h>

// This code works on any and all Arduino or Arduino-compatible boards. Set these pins to any
// `digitalWrite()`-capable pin number as you see fit. Using pins 0 and 1, however, is generally not
// recommended, as they are for serial communication to upload your program. Therefore, for the
// Uno/Nano/Pro Mini type ATmega328-based boards, I recommend Digital pins 2 through 13, inclusive,
// or Analog pins A0 through A5, inclusive. NB: If your ATmega168/328-based board has an A6 or A7
// pin, these pins do NOT support `digitalWrite()`/`digitalRead()`!
constexpr uint8_t CS_PIN = 2;
constexpr uint8_t INC_PIN = 3;
constexpr uint8_t UP_DOWN_PIN = 4;

X9C_digital_pot pot(CS_PIN, INC_PIN, UP_DOWN_PIN);

void setup()
{
    Serial.begin(115200);

    pot.setWiperVoltage(2.5);
    uint8_t wiper_position = pot.getWiperPosition();

    delay(5000);

    wiper_position += 10;
    pot.setWiperPosition(wiper_position);

    float wiper_voltage = pot.getWiperCommandedVoltage();
    Serial.print(F("Commanded output voltage = "));
    Serial.println(wiper_voltage);
}

void loop()
{
}
