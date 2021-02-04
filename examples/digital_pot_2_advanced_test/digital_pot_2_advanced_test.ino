/*
Gabriel Staples

Advanced demo of this library.

Circuit:
X9C IC Pin  Arduino Pin   Description
----------  -----------   -----------
Inc         Set in code   Increment
U/D         Set in code   Up/Down selection
VH          5V            Potentiometer High side input
Vss         GND           IC GND
Vcc         5V            IC Power
CS          Set in code   Chip Select
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

// Calibrate this! Measure the actual voltage at the potentiometer's VH pin with a multimeter and
// change this value to that value to calibrate the commanded voltage value at the wiper!
constexpr float WIPER_HIGH_VOLTAGE = 5.0;

X9C_digital_pot pot(CS_PIN, INC_PIN, UP_DOWN_PIN, WIPER_HIGH_VOLTAGE);

void setup()
{
    Serial.begin(115200);

    Serial.println(F("Commanding wiper voltage to 2.5V."));
    pot.setWiperVoltage(2.5);
    uint8_t wiper_position = pot.getWiperPosition();

    Serial.print(F("wiper_position = "));
    Serial.println(wiper_position);
    Serial.println();
    Serial.println(
        F("Delaying 5 seconds to let you verify the output voltage with a multimeter..."));
    delay(5000);

    // Now cycle through the wiper outputs with uint8_t **absolute commands** incrementing and
    // overflowing many many times to ensure indexing doesn't get out-of-sync inside the chip.
    // The user should measure the output voltages before and after this point with a multimeter
    // to see if they are accurate.
    constexpr uint32_t NUM_CMDS = 256 * 1000UL;
    Serial.print(F("Running "));
    Serial.print(NUM_CMDS);
    Serial.println(F(" potentiometer wiper commands as fast as possible."));
    for (uint32_t i = 0; i < NUM_CMDS; i++)
    {
        static uint8_t wiper_cmd = 0;
        pot.setWiperPosition(wiper_cmd);
        wiper_cmd++;
    }
}

void loop()
{
    static uint8_t wiper_position_cmd = 0;

    Serial.print(F("Commanding wiper position to "));
    Serial.print(wiper_position_cmd);

    pot.setWiperPosition(wiper_position_cmd);
    wiper_position_cmd++;
    wiper_position_cmd = wiper_position_cmd % 110;  // wrap around at 110 just for kicks

    uint8_t wiper_position_actual = pot.getWiperPosition();
    float commanded_voltage = pot.getWiperCommandedVoltage();

    Serial.print(F(". Actual wiper position = "));
    Serial.print(wiper_position_actual);
    Serial.print(F(". Expected wiper voltage = "));
    Serial.print(commanded_voltage);
    Serial.println(F(" V."));

    delay(3000);  // give the user time to measure and verify each output voltage with a multimeter
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
