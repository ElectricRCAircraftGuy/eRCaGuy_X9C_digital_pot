
#include "eRCaGuy_X9C_digital_pot.h"

#define SELECT_CHIP() digitalWrite(_CS_PIN, LOW)
#define DESELECT_CHIP() digitalWrite(_CS_PIN, HIGH)

#define TOGGLE_INC_PIN_HIGH_TO_LOW()  \
    do                                \
    {                                 \
        digitalWrite(_INC_PIN, HIGH); \
        digitalWrite(_INC_PIN, LOW);  \
    } while (false)

X9C103::X9C103(uint8_t cs_pin, uint8_t inc_pin, uint8_t up_down_pin) :
    _CS_PIN(cs_pin),
    _INC_PIN(inc_pin),
    _UP_DOWN_PIN(up_down_pin)
{
    pinMode(_CS_PIN, OUTPUT);
    pinMode(_INC_PIN, OUTPUT);
    pinMode(_UP_DOWN_PIN, OUTPUT);
}

void X9C103::wiperUp()
{
    SELECT_CHIP();
    digitalWrite(_UP_DOWN_PIN, HIGH);
    TOGGLE_INC_PIN_HIGH_TO_LOW();
    DESELECT_CHIP();
    _wiper_pos++;
}

void X9C103::wiperDown()
{
    SELECT_CHIP();
    digitalWrite(_UP_DOWN_PIN, LOW);
    TOGGLE_INC_PIN_HIGH_TO_LOW();
    DESELECT_CHIP();
    _wiper_pos--;
}

void X9C103::storeWiperPosition()
{
    // Ensure the proper time delay from the last call has elapsed, assuming rapid-fire
    // multiple calls to this function
    uint32_t t_now_ms = millis();
    uint32_t t_delta_ms = t_now_ms - _time_last_stored_ms;
    if (t_delta_ms < NON_VOLATILE_MEMORY_STORE_TIME_MS)
    {
        delay(t_delta_ms);
    }
    _time_last_stored_ms = t_now_ms;

    // Store wiper position into non-volatile memory, per the datasheet
    digitalWrite(_INC_PIN, HIGH);
    SELECT_CHIP();
    DESELECT_CHIP();
}

void X9C103::setWiperPosition(uint8_t position)
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

void X9C103::indexPosition()
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
