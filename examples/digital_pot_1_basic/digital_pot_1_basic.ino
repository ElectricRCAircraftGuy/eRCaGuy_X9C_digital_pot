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

constexpr uint8_t CS_PIN = 2;
constexpr uint8_t INC_PIN = 3;
constexpr uint8_t UP_DOWN_PIN = 4;

X9C_digital_pot pot(CS_PIN, INC_PIN, UP_DOWN_PIN);

void setup()
{
    pot.setWiperVoltage(2.5);
    uint8_t wiper_position = pot.getWiperPosition();

    delay(5000);

    wiper_position += 10;
    pot.setWiperPosition(wiper_position);

    float wiper_voltage = pot.getWiperCommandedVoltage();
}

void loop()
{
}
