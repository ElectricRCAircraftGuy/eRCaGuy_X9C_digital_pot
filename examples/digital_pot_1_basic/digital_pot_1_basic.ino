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
    Serial.begin(115200);

    Serial.println(F("Setting wiper voltage to 2.5V."))
    pot.setWiperVoltage(2.5);
    uint8_t wiper_position = pot.getWiperPosition();

    Serial.print(F("wiper_position = "));
    Serial.println(wiper_position);
    Serial.println();

    delay(5000);
}

void loop()
{
    static uint8_t wiper_position = pot.getWiperPosition();

    Serial.print(F("Setting wiper position to "));
    Serial.println(wiper_position);
    pot.setWiperPosition(wiper_position);
    wiper_position++;
    delay(1000);
}
