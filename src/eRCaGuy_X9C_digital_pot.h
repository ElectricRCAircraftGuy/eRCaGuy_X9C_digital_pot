#pragma once

#include <Arduino.h>

class X9C_digital_pot
{
public:
    /// Constructor
    X9C_digital_pot(uint8_t cs_pin, uint8_t inc_pin, uint8_t up_down_pin);

    /// Move the wiper up the specified number of increments (default 1).
    void wiperUp(uint8_t num_increments = 1);

    /// Move the wiper down the specified number of increments (default 1).
    void wiperDown(uint8_t num_decrements = 1);

    /// Store the current wiper position into non-volatile memory.
    void storeWiperPosition();

    /// Set an absolute wiper position from 1 to 100 (default middle (50)).
    void setWiperPosition(uint8_t position = 50);

    /// Calibrate (index) the absolute position and go to the specified position (default to the
    /// middle position (50))
    void indexPosition(uint8_t position = 50);

    /// How long you must wait for the non-volatile memory to get stored (from the datasheet:
    /// `t_CPH`)
    static constexpr uint8_t NON_VOLATILE_MEMORY_STORE_TIME_MS = 20;
    /// Minimum absolute wiper position allowed
    static constexpr int16_t WIPER_MINIMUM = 1;
    /// Maximum absolute wiper position allowed
    static constexpr int16_t WIPER_MAXIMUM =  100;

private:
    /// timestamp of the last time the wiper position was stored into non-volatile memory
    uint32_t _time_last_stored_ms = 0 - NON_VOLATILE_MEMORY_STORE_TIME_MS;  // intentional underflow

    /// Chip Select pin
    const uint8_t _CS_PIN;
    /// Incremement/Decrement pin
    const uint8_t _INC_PIN;
    /// Up/Down selection pin (affects which direction the Inc pin works)
    const uint8_t _UP_DOWN_PIN;

    /// Current absolute wiper position (1 to 100); use signed 16-bit value to allow for proper
    /// math in the library
    int16_t _wiper_pos = 0;

    /// Set to true if the position has been indexed, meaning absolute
    /// position commands are now possible.
    bool _is_indexed = false;
};
