
#include "eRCaGuy_X9C_digital_pot.h"

#define TOGGLE_INC_PIN_HIGH_TO_LOW()  \
    do                                \
    {                                 \
        digitalWrite(_INC_PIN, HIGH); \
        digitalWrite(_INC_PIN, LOW);  \
    } while (false)

X9C_digital_pot::X9C_digital_pot(uint8_t cs_pin, uint8_t inc_pin, uint8_t up_down_pin,
                                 float wiper_high_voltage) :
    _CS_PIN(cs_pin),
    _INC_PIN(inc_pin),
    _UP_DOWN_PIN(up_down_pin),
    _WIPER_HIGH_VOLTAGE(wiper_high_voltage)
{
    pinMode(_CS_PIN, OUTPUT);
    pinMode(_INC_PIN, OUTPUT);
    pinMode(_UP_DOWN_PIN, OUTPUT);

    standBy();
}

void X9C_digital_pot::wiperUp(uint8_t num_increments)
{
    if (num_increments == 0)
    {
        return;
    }
    else if (num_increments > WIPER_MAXIMUM)
    {
        num_increments = WIPER_MAXIMUM;
    }

    // "Mode Selection" table in datasheet on p7 shows that if Inc pin is LOW when CS is driven LOW
    // then the wiper will move up or down according to the Up/Down pin state. So, set Up/Down pin
    // state FIRST, and update the `_wiper_pos` and `num_increments` values accordingly
    // right after selecting the chip!
    digitalWrite(_UP_DOWN_PIN, HIGH);
    selectChip();
    _wiper_pos++;
    num_increments--;

    for (uint8_t i = 0; i < num_increments; i++)
    {
        TOGGLE_INC_PIN_HIGH_TO_LOW();
        _wiper_pos++;
    }

    deselectChip();  // also returns to standby since Inc pin is LOW

    if (_wiper_pos > WIPER_MAXIMUM)
    {
        _wiper_pos = WIPER_MAXIMUM;
    }
}

void X9C_digital_pot::wiperDown(uint8_t num_decrements)
{
    if (num_decrements == 0)
    {
        return;
    }
    else if (num_decrements > WIPER_MAXIMUM)
    {
        num_decrements = WIPER_MAXIMUM;
    }

    // "Mode Selection" table in datasheet on p7 shows that if Inc pin is LOW when CS is driven LOW
    // then the wiper will move up or down according to the Up/Down pin state. So, set Up/Down pin
    // state FIRST, and update the `_wiper_pos` and `num_decrements` values accordingly
    // right after selecting the chip!
    digitalWrite(_UP_DOWN_PIN, LOW);
    selectChip();
    _wiper_pos--;
    num_decrements--;

    for (uint8_t i = 0; i < num_decrements; i++)
    {
        TOGGLE_INC_PIN_HIGH_TO_LOW();
        _wiper_pos--;
    }

    deselectChip();  // also returns to standby since Inc pin is LOW

    if (_wiper_pos < WIPER_MINIMUM)
    {
        _wiper_pos = WIPER_MINIMUM;
    }
}

void X9C_digital_pot::standBy()
{
    ensureStoreHasCompleted();
    digitalWrite(_INC_PIN, LOW);
    digitalWrite(_CS_PIN, HIGH);
}

void X9C_digital_pot::storeWiperPosition()
{
    // Store wiper position into non-volatile memory, per the datasheet "Mode Selection" table on
    // p7. NB: the 20ms required store delay where CS and Inc must both remain HIGH (see the "AC
    // Timing Diagram" in the datasheet on p6) is enforced inside the `selectChip()` function (and
    // `standBy()` function too) which is called here and inside all other routines which could
    // interfere with the stored delay.
    digitalWrite(_INC_PIN, HIGH);
    selectChip();
    deselectChip();

    _time_last_store_started_ms = millis();
    _last_store_complete = false;
}

void X9C_digital_pot::setWiperPosition(uint8_t position)
{
    position = constrain(position, WIPER_MINIMUM, WIPER_MAXIMUM);

    if (!_is_indexed)
    {
        indexPosition(position);
        return;
    }

    int16_t delta = position - _wiper_pos;  // desired - actual

    if (delta > 0)
    {
        wiperUp(delta);
    }
    else
    {
        delta = -delta;
        wiperDown(delta);
    }
}

uint8_t X9C_digital_pot::getWiperPosition()
{
    return (uint8_t)_wiper_pos;
}

void X9C_digital_pot::setWiperVoltage(float volts)
{
    volts = constrain(volts, 0, _WIPER_HIGH_VOLTAGE);
    uint8_t wiper_cmd =
        round(volts / _WIPER_HIGH_VOLTAGE * (WIPER_MAXIMUM - WIPER_MINIMUM) + WIPER_MINIMUM);
    setWiperPosition(wiper_cmd);
}

float X9C_digital_pot::getWiperCommandedVoltage()
{
    float cmd_voltage =
        (float)(_wiper_pos - WIPER_MINIMUM) / (WIPER_MAXIMUM - WIPER_MINIMUM) * _WIPER_HIGH_VOLTAGE;
    return cmd_voltage;
}

void X9C_digital_pot::indexPosition(uint8_t position)
{
    position = constrain(position, WIPER_MINIMUM, WIPER_MAXIMUM);

    // The pot only has 100 increments (see Datasheet p1: "100 Wiper Tap Points"), so we can
    // guarantee that the lowest position (1 of 100) is reached simply by going down 100 counts.
    wiperDown(WIPER_MAXIMUM);
    _is_indexed = true;
    _wiper_pos = 1;

    // set to commanded position to start; do NOT call `setWiperPosition()` here or else we will
    // engage in recursion, which is discouraged for low-memory embedded devices

    // desired - actual; guaranteed to be zero or positive since `position` is constrained to 1 to
    // 100 and `_wiper_pos` is 1
    int16_t delta = position - _wiper_pos;
    wiperUp(delta);
}

// ----------------- PRIVATE -------------------

void X9C_digital_pot::selectChip()
{
    ensureStoreHasCompleted();
    digitalWrite(_CS_PIN, LOW);
}

void X9C_digital_pot::deselectChip()
{
    digitalWrite(_CS_PIN, HIGH);
}

void X9C_digital_pot::ensureStoreHasCompleted()
{
    if (!_last_store_complete)
    {
        uint32_t t_now_ms = millis();
        uint32_t t_elapsed_ms = t_now_ms - _time_last_store_started_ms;
        if (t_elapsed_ms < NON_VOLATILE_MEMORY_STORE_TIME_MS)
        {
            uint32_t t_remaining_ms = NON_VOLATILE_MEMORY_STORE_TIME_MS - t_elapsed_ms;
            delay(t_remaining_ms);
        }
        _last_store_complete = true;
    }
}
