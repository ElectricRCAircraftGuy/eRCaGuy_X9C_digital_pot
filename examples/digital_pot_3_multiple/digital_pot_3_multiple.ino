/*
Gabriel Staples

Multiple potentiometer demo of this library.

Let's imagine we have potentiometers which control a
radio-controlled (RC) vehicle somehow by replacing the control sticks of the controller, for
instance. Let's imagine it's an RC car with one digital potentiometer for throttle and one for
steering.

Circuit for each digital potentiometer:
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

// Adjust or reverse these as necessary for your setup
#define THROTTLE_UP()  throttle.wiperUp()
#define THROTTLE_DOWN() throttle.wiperDown()
#define THROTTLE_SET(val) throttle.setWiperPosition(val)

// Adjust or reverse these as necessary for your setup
#define STEER_LEFT()  steering.wiperUp()
#define STEER_RIGHT() steering.wiperDown()
#define STEERING_SET(val) steering.setWiperPosition(val)

// This code works on any and all Arduino or Arduino-compatible boards. Set these pins to any
// `digitalWrite()`-capable pin number as you see fit. Using pins 0 and 1, however, is generally not
// recommended, as they are for serial communication to upload your program. Therefore, for the
// Uno/Nano/Pro Mini type ATmega328-based boards, I recommend Digital pins 2 through 13, inclusive,
// or Analog pins A0 through A5, inclusive. NB: If your ATmega168/328-based board has an A6 or A7
// pin, these pins do NOT support `digitalWrite()`/`digitalRead()`!
constexpr X9C_digital_pot::Pins THROTTLE_POT_PINS =
{
    .cs_pin = 10,
    .inc_pin = 4,
    .up_down_pin = 5,
};
constexpr X9C_digital_pot::Pins STEERING_POT_PINS =
{
    .cs_pin = 11,
    .inc_pin = A0,
    .up_down_pin = A1,
};

// Calibrate this! Measure the actual voltage at the potentiometer's VH pin with a multimeter and
// change this value to that value to calibrate the commanded voltage value at the wiper!
constexpr float WIPER_HIGH_VOLTAGE = 5.0;

X9C_digital_pot throttle(THROTTLE_POT_PINS, WIPER_HIGH_VOLTAGE);
X9C_digital_pot steering(STEERING_POT_PINS, WIPER_HIGH_VOLTAGE);

void setup()
{
    // Crude demo just to show how to call the macros/functions. Adjust for your needs.

    THROTTLE_UP();
    THROTTLE_DOWN();
    THROTTLE_SET(50); // 1 to 100

    STEER_LEFT();
    STEER_RIGHT();
    STEERING_SET(75); // 1 to 100, where 50 is center, and 1 is either full left or full right, and 100 is the opposite
}

void loop()
{
}
