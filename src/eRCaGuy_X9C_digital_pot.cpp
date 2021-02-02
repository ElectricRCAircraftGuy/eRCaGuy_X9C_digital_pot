
#include "eRCaGuy_X9C_digital_pot.h"


#define TOGGLE_INC_PIN_HIGH_TO_LOW()  \
    do                                \
    {                                 \
        digitalWrite(_INC_PIN, HIGH); \
        digitalWrite(_INC_PIN, LOW);  \
    } while (false)

X9C_digital_pot::X9C_digital_pot(uint8_t cs_pin, uint8_t inc_pin, uint8_t up_down_pin) :
    _CS_PIN(cs_pin),
    _INC_PIN(inc_pin),
    _UP_DOWN_PIN(up_down_pin)
{
    pinMode(_CS_PIN, OUTPUT);
    pinMode(_INC_PIN, OUTPUT);
    pinMode(_UP_DOWN_PIN, OUTPUT);
}

void X9C_digital_pot::wiperUp(uint8_t num_increments = 1)
{
    selectChip();
    digitalWrite(_UP_DOWN_PIN, HIGH);
    TOGGLE_INC_PIN_HIGH_TO_LOW();
    deselectChip();
    _wiper_pos += num_increments;
    if (_wiper_pos > WIPER_MAXIMUM) {
        _wiper_pos = WIPER_MAXIMUM;
    }
}

void X9C_digital_pot::wiperDown(uint8_t num_decrements = 1)
{
    selectChip();
    digitalWrite(_UP_DOWN_PIN, LOW);
    TOGGLE_INC_PIN_HIGH_TO_LOW();
    deselectChip();
    _wiper_pos -= num_decrements;
    if (_wiper_pos < WIPER_MINIMUM) {
        _wiper_pos = WIPER_MINIMUM;
    }
}

void X9C_digital_pot::storeWiperPosition()
{
    // Ensure the proper time delay from the last call has elapsed, assuming rapid-fire
    // multiple calls to this function
    uint32_t t_now_ms = millis();
    uint32_t t_delta_ms = t_now_ms - _time_last_store_started_ms  if (t_delta_ms < NON_VOLATILE_MEMORY_STORE_TIME_MS)
    {
        delay(t_delta_ms);
    }
    _time_last_store_started_ms = millis();

    // Store wiper position into non-volatile memory, per the datasheet "Mode Selection" table on p7
    digitalWrite(_INC_PIN, HIGH);
    selectChip();
    deselectChip();

    _last_store_complete = false;
}

void X9C_digital_pot::setWiperPosition(uint8_t position)
{
    if (!_is_indexed)
    {
        indexPosition();
    }

    int16_t delta = position - _wiper_pos;  // desired - actual

    if (delta > 0)
    {
        for (uint8_t i = 0; i < delta; i++)
        {
            wiperUp();
        }
    }
    else
    {
        delta = -delta;
        for (uint8_t i = 0; i < delta; i++)
        {
            wiperDown();
        }
    }
}

void X9C_digital_pot::indexPosition()
{
    // The pot only has 100 increments (see Datasheet p1: "100 Wiper Tap Points"), so we can
    // guarantee that the lowest position (1 of 100) is reached simply by going down 100 counts.
    for (uint8_t i = 0; i < 100; i++)
    {
        wiperDown();
    }
    _is_indexed = true;
    _wiper_pos = 1;

    // set to default position of 50
    while (_wiper_pos < 50)
    {
        wiperUp();
    }
}

// ----------------- PRIVATE ----------------

void X9C_digital_pot::selectChip()
{
    if (!_last_store_complete)
    {

    }
    _last_store_complete = true;

    digitalWrite(_CS_PIN, LOW);
}

void X9C_digital_pot::deselectChip()
{
    digitalWrite(_CS_PIN, HIGH);
}
