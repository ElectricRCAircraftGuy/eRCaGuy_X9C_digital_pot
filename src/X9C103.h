#ifndef X9C103_h
#define X9C103_h

#pragma once

#include <Arduino.h>

class X9C103
{
public:
    /// Constructor
    X9C103(uint8_t cs_pin, uint8_t inc_pin, uint8_t up_down_pin);

    /// Move the wiper up 1 increment.
    void wiperUp();

    /// Move the wiper down 1 increment.
    void wiperDown();

    /// Store the current wiper position into non-volatile memory.
    void storeWiperPosition();

    /// Set an absolute wiper position from 1 to 100.
    void setWiperPosition(uint8_t position);

    /// Calibrate (index) the absolute position and go to the middle position (50)
    void indexPosition();

private:

    /// How long you must wait for the non-volatile memory to get stored
    constexpr uint8_t NON_VOLATILE_MEMORY_STORE_TIME_MS = 20;
    /// timestamp of the last time the wiper position was stored into non-volatile memory
    uint32_t time_last_stored_ms = 0 - NON_VOLATILE_MEMORY_STORE_TIME_MS;

    constexpr uint8_t _CS_PIN;
    constexpr uint8_t _INC_PIN;
    constexpr uint8_t _UP_DOWN_PIN;

    /// Current absolute wiper position (1 to 100)
    uint8_t _wiper_pos = 0;

    /// Set to true if the position has been indexed, meaning absolute
    /// position commands are now possible.
    bool _is_indexed = false;
}

#endif
