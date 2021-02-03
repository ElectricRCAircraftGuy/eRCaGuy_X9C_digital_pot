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

    Serial.println(F("Commanding wiper voltage to 2.5V."));
    pot.setWiperVoltage(2.5);
    uint8_t wiper_position = pot.getWiperPosition();

    Serial.print(F("wiper_position = "));
    Serial.println(wiper_position);
    Serial.println();

    delay(5000);
}

void loop()
{
    static uint8_t wiper_position_cmd = pot.getWiperPosition();

    Serial.print(F("Commanding wiper position to "));
    Serial.print(wiper_position_cmd);

    pot.setWiperPosition(wiper_position_cmd);
    wiper_position_cmd++;

    uint8_t wiper_position_actual = pot.getWiperPosition();
    float commanded_voltage = pot.getWiperCommandedVoltage();

    Serial.print(F(". Actual wiper position = "));
    Serial.print(wiper_position_actual);
    Serial.print(F(". Expected wiper voltage = "));
    Serial.print(commanded_voltage);
    Serial.println(F(" V."));

    delay(2000);
}

/*
Sample output:


    Commanding wiper position to 249. Actual wiper position = 100. Expected wiper voltage = 5.00 V.
    Commanding wiper position to 250. Actual wiper position = 100. Expected wiper voltage = 5.00 V.
    Commanding wiper position to 251. Actual wiper position = 100. Expected wiper voltage = 5.00 V.
    Commanding wiper position to 252. Actual wiper position = 100. Expected wiper voltage = 5.00 V.
    Commanding wiper position to 253. Actual wiper position = 100. Expected wiper voltage = 5.00 V.
    Commanding wiper position to 254. Actual wiper position = 100. Expected wiper voltage = 5.00 V.
    Commanding wiper position to 255. Actual wiper position = 100. Expected wiper voltage = 5.00 V.
    Commanding wiper position to 0. Actual wiper position = 1. Expected wiper voltage = 0.00 V.
    Commanding wiper position to 1. Actual wiper position = 1. Expected wiper voltage = 0.00 V.
    Commanding wiper position to 2. Actual wiper position = 2. Expected wiper voltage = 0.05 V.
    Commanding wiper position to 3. Actual wiper position = 3. Expected wiper voltage = 0.10 V.
    Commanding wiper position to 4. Actual wiper position = 4. Expected wiper voltage = 0.15 V.
    Commanding wiper position to 5. Actual wiper position = 5. Expected wiper voltage = 0.20 V.
    Commanding wiper position to 6. Actual wiper position = 6. Expected wiper voltage = 0.25 V.
    Commanding wiper position to 7. Actual wiper position = 7. Expected wiper voltage = 0.30 V.
    Commanding wiper position to 8. Actual wiper position = 8. Expected wiper voltage = 0.35 V.
    Commanding wiper position to 9. Actual wiper position = 9. Expected wiper voltage = 0.40 V.
    Commanding wiper position to 10. Actual wiper position = 10. Expected wiper voltage = 0.45 V.
    Commanding wiper position to 11. Actual wiper position = 11. Expected wiper voltage = 0.51 V.
    Commanding wiper position to 12. Actual wiper position = 12. Expected wiper voltage = 0.56 V.
    Commanding wiper position to 13. Actual wiper position = 13. Expected wiper voltage = 0.61 V.
    Commanding wiper position to 14. Actual wiper position = 14. Expected wiper voltage = 0.66 V.
    Commanding wiper position to 15. Actual wiper position = 15. Expected wiper voltage = 0.71 V.
    Commanding wiper position to 16. Actual wiper position = 16. Expected wiper voltage = 0.76 V.
    Commanding wiper position to 17. Actual wiper position = 17. Expected wiper voltage = 0.81 V.

*/
