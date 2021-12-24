#include "XE_TCA95x5.h"

/**
 * Read a specified number of bytes using the I2C bus.
 * @param output: The buffer in which to store the read values.
 * @param address: Register address to read (or starting address in burst reads)
 * @param length: Number of bytes to read.
 */
bool TCA95x5::read(uint8_t *output, tca95x5_reg_address_t address, uint8_t length) {
    bool success = true;
    Wire.beginTransmission(_device_address);
    Wire.write(address);
    if (Wire.endTransmission() != 0)
        success = false;

    else  // OK, all worked, keep going
    {
        Wire.requestFrom(_device_address, uint8_t(length));
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
    Wire.beginTransmission(_device_address);
    Wire.write(address);
    for (size_t i = 0; i < length; i++) {
        Wire.write(input[i]);
    }

    if (Wire.endTransmission() != 0) {
        success = false;
    }
    return success;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TCA95x5::begin(uint8_t address) { 
    _device_address = address;
    Wire.begin();  
    Wire.beginTransmission(_device_address); //Test if device present 
    if(Wire.endTransmission() != 0) 
        return 0;
    else 
        return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TCA95x5::write(tca95x5_config_t config) {
    write(config.mode);
    write(config.output);
    write(config.polarity);
}

void TCA95x5::write(tca95x5_mode_config_t config) { write((uint8_t *)&config, TCA95x5_REGISTER::CONFIG_0); }

void TCA95x5::write(tca95x5_output_config_t config) { write((uint8_t *)&config, TCA95x5_REGISTER::OUTPUT_0); }

void TCA95x5::write(tca95x5_polarity_config_t config) { write((uint8_t *)&config, TCA95x5_REGISTER::POLARITY_INV_0); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TCA95x5::read(tca95x5_config_t &config) {
    read(config.mode);
    read(config.output);
    read(config.polarity);
}

void TCA95x5::read(tca95x5_mode_config_t &config) { read((uint8_t *)&config, TCA95x5_REGISTER::CONFIG_0); }

void TCA95x5::read(tca95x5_output_config_t &config) { read((uint8_t *)&config, TCA95x5_REGISTER::OUTPUT_0); }

void TCA95x5::read(tca95x5_polarity_config_t &config) { read((uint8_t *)&config, TCA95x5_REGISTER::POLARITY_INV_0); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TCA95x5::read(tca95x5_input_status_t &status) { read((uint8_t *)&status, TCA95x5_REGISTER::INPUT_0); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

tca95x5_config_t TCA95x5::getConfig() {
    tca95x5_config_t config;
    read(config);
    return config;
}

tca95x5_mode_config_t TCA95x5::getModeConfig() {
    tca95x5_mode_config_t config;
    read(config);
    return config;
}

tca95x5_output_config_t TCA95x5::getOutputConfig() {
    tca95x5_output_config_t config;
    read(config);
    return config;
}

tca95x5_polarity_config_t TCA95x5::getPolarityConfig() {
    tca95x5_polarity_config_t config;
    read(config);
    return config;
}

tca95x5_input_status_t TCA95x5::getInputStatus() {
    tca95x5_input_status_t status;
    read(status);
    return status;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TCA95x5::pinMode(tca95x5_config_t &config, size_t pin_id, tca95x5_pinMode_t mode) { 
    bitWrite(config.mode.raw, pin_id, mode); 
}

void TCA95x5::digitalWrite(tca95x5_config_t &config, size_t pin_id, tca95x5_pin_output_state_t state) { 
    bitWrite(config.output.raw, pin_id, state);
}


void TCA95x5::pinMode(int pin, int mode) {
    tca95x5_mode_config_t modeConfig = getModeConfig();

    if(mode == INPUT) {
        modeConfig.raw |= (1 << pin);
    }
    else {
        modeConfig.raw &= ~(1 << pin);
    }

    write(modeConfig);

}

bool TCA95x5::digitalRead(int pin) {
    tca95x5_input_status_t inputStatus = getInputStatus();

    return (inputStatus.raw >> pin) & 1;
}

void TCA95x5::digitalWrite(int pin, int status) {
    tca95x5_output_config_t outputConfig;

    outputConfig = getOutputConfig();

    if(status == LOW) {
        outputConfig.raw &= ~(1 << pin);
    }
    else {
        outputConfig.raw |= (1 << pin);
    }

    write(outputConfig);
}
