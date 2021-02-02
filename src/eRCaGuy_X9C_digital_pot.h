#pragma once

#include <Arduino.h>

class X9C_digital_pot
{
public:
    /// \brief      Constructor
    /// \param[in]  cs_pin             Chip Select pin
    /// \param[in]  inc_pin            Increment pin
    /// \param[out] up_down_pin        Up/Down pin
    /// \param[out] reference_voltage  Vcc reference voltage--determines the max output voltage at
    ///                                the potentiometer's wiper
    /// \return     NA
    X9C_digital_pot(uint8_t cs_pin, uint8_t inc_pin, uint8_t up_down_pin,
                    float reference_voltage = 5.0);

    /// Move the wiper up the specified number of increments (default 1; allowed: 0 to 100).
    void wiperUp(uint8_t num_increments = 1);

    /// Move the wiper down the specified number of increments (default 1; allowed: 0 to 100).
    void wiperDown(uint8_t num_decrements = 1);

    /// Return to standby; do NOT store the current wiper position into non-volatile memory
    void standBy();

    /// Store the current wiper position into non-volatile memory.
    void storeWiperPosition();

    /// Set an absolute wiper position from 1 to 100 (default middle (50)).
    void setWiperPosition(uint8_t position = 50);

    /// Get the current wiper position, from 1 to 100. Will NOT be a valid value if
    /// `setWiperPosition()` or `indexPosition()` has not been called at least once PRIOR to this
    /// call.
    uint8_t getWiperPosition();

    /// Set the proper wiper position setting to obtain this desired voltage at the wiper; this is
    /// an open-loop command.
    void setWiperVoltage(float volts);

    /// Get the commanded voltage at the wiper based on the last commanded wiper position.
    float getWiperVoltage();

    /// Calibrate (index) the absolute position and go to the specified position from 1 to 100
    /// (default to the middle position (50))
    void indexPosition(uint8_t position = 50);

    /// How long you must wait for the non-volatile memory to get stored (from the datasheet:
    /// `t_CPH`)
    static constexpr uint8_t NON_VOLATILE_MEMORY_STORE_TIME_MS = 20 + 1;  // + 1 for good measure
    /// Minimum absolute wiper position allowed
    static constexpr int16_t WIPER_MINIMUM = 1;
    /// Maximum absolute wiper position allowed; also equal to the total number of wiper positions,
    /// since it is 1-indexed
    static constexpr int16_t WIPER_MAXIMUM = 100;

private:
    /// Select the chip, ensuring that the proper store time has elapsed FIRST BEFORE the
    /// chip select operation is allowed, as selecting the chip too soon will cancel a recent
    /// non-volatile store attempt, according to the "AC Timing Diagram" in the datasheet.
    void selectChip();

    /// Deselect the chip
    void deselectChip();

    /// Ensure proper store time has elapsed for any pending store operation BEFORE allowing Inc OR
    /// CS pins to go LOW again. See the "AC Timing Diagram" in the datasheet on p6--in particular
    /// the t_CPH variable.
    void ensureStoreHasCompleted();

    /// timestamp of the start of the last store attempt of the the wiper position into non-volatile
    /// memory; initialize with intentional underflow so it will appear the full time has elapsed
    /// the very first call
    uint32_t _time_last_store_started_ms = 0 - NON_VOLATILE_MEMORY_STORE_TIME_MS;
    /// set to true once we are sure the last store operation is complete
    bool _last_store_complete = false;

    /// Chip Select pin
    const uint8_t _CS_PIN;
    /// Incremement/Decrement pin
    const uint8_t _INC_PIN;
    /// Up/Down selection pin (affects which direction the Inc pin works)
    const uint8_t _UP_DOWN_PIN;
    /// The reference voltage, Vcc. At the max potentiometer setting this is the output voltage
    /// at the wiper
    const float _REFERENCE_VOLTAGE;

    /// Current absolute wiper position (1 to 100); use signed 16-bit value to allow for proper
    /// math in the library
    int16_t _wiper_pos = 0;

    /// Set to true if the position has been indexed, meaning absolute
    /// position commands are now possible.
    bool _is_indexed = false;
};
