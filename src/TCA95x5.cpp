#include "TCA95x5.h"

/**
 * Read a specified number of bytes using the I2C bus.
 * @param output: The buffer in which to store the read values.
 * @param address: Register address to read (or starting address in burst reads)
 * @param length: Number of bytes to read.
 */
bool TCA95x5::read(uint8_t *output, tca95x5_reg_address_t address, uint8_t length) {
    bool success = true;
    Wire.beginTransmission(address);
    Wire.write(address);
    if (Wire.endTransmission() != 0)
        success = false;

    else  // OK, all worked, keep going
    {
        Wire.requestFrom(address, length);
        for (size_t i = 0; (i < length) and Wire.available(); i++) {
            uint8_t c = Wire.read();
            output[i] = c;
        }
    }
    return success;
}

/**
 * Write a value to a register using I2C
 *
 * @param input: Byte to write to the register.
 * @param address: Address of register to write to.
 * @return: Success/error success of the write.
 */
bool TCA95x5::write(uint8_t *input, tca95x5_reg_address_t address, uint8_t length) {
    bool success = true;
    Wire.beginTransmission(device_address);
    Wire.write(address);
    for (size_t i = 0; i < length; i++) {
        Wire.write(input[i]);
    }

    if (Wire.endTransmission() != 0) {
        success = false;
    }
    return success;
}

void TCA95x5::reset_config() {
    config.mode.raw = 0xFFFF;
    config.output_state.raw = 0xFFFF;
    config.polarity.raw = 0x0000;
}

/**
 *
 */
void TCA95x5::apply_config() {
    apply_state_config();
    apply_mode_config();
    apply_polarity_config();
}

/**
 *
 */
void TCA95x5::apply_mode_config() { write((uint8_t *)&config.mode.raw, TCA95x5_REGISTER::TCA95x5_CONFIG_0); }

/**
 *
 */
void TCA95x5::apply_state_config() { write((uint8_t *)&config.output_state.raw, TCA95x5_REGISTER::TCA95x5_OUTPUT_0); }

/**
 *
 */
void TCA95x5::apply_polarity_config() {
    write((uint8_t *)&config.polarity.raw, TCA95x5_REGISTER::TCA95x5_POLARITY_INV_0);
}

tca95x5_config_t TCA95x5::get_active_config() {
    tca95x5_config_t active_config;
    read((uint8_t *)&active_config.mode.raw, TCA95x5_REGISTER::TCA95x5_CONFIG_0);
    read((uint8_t *)&active_config.output_state.raw, TCA95x5_REGISTER::TCA95x5_OUTPUT_0);
    read((uint8_t *)&active_config.polarity.raw, TCA95x5_REGISTER::TCA95x5_POLARITY_INV_0);
    return active_config;
}

void TCA95x5::set_pin_mode(size_t pin_number, tca95x5_pin_mode_t mode) {
    config.mode.raw = bitWrite(config.mode.raw, pin_number, (bool)mode);
}

void TCA95x5::set_pin_output_state(size_t pin_number, tca95x5_pin_output_state_t state) {
    config.output_state.raw = bitWrite(config.output_state.raw, pin_number, (bool)state);
}