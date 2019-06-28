#include <Arduino.h>
#include <Wire.h>

/**
 * Register map of the IO expander.
 */
typedef enum TCA95x5_REGISTER {
    TCA95x5_INPUT_0 = 0x00,         // Actual pin states for port 0
    TCA95x5_INPUT_1 = 0x01,         // Actual pin states for port 1
    TCA95x5_OUTPUT_0 = 0x02,        // Output states for port 0
    TCA95x5_OUTPUT_1 = 0x03,        // Output states for port 1
    TCA95x5_POLARITY_INV_0 = 0x04,  // Polarity for inputs on port 0
    TCA95x5_POLARITY_INV_1 = 0x05,  // Polarity for inputs on port 1
    TCA95x5_CONFIG_0 = 0x06,        // Pin mode/direction for port 0
    TCA95x5_CONFIG_1 = 0x07,        // Pin mode/direction for port 1
} tca95x5_reg_address_t;

/**
 * Directionality for I/O pins
 * Pins are initially configured as inputs upon restart.
 */
typedef enum TCA95x5_PIN_MODE {
    TCA95x5_INPUT = 1,
    TCA95x5_OUTPUT = 0,
} tca95x5_pin_mode_t;

/**
 * Output state of pins.
 * This configuration does not affect pins configured as inputs.
 */
typedef enum TCA95x5_PIN_STATE {
    TCA95x5_LOW = 0,
    TCA95x5_HIGH = 1,
} tca95x5_pin_output_state_t;

/**
 * Polarity of input pins.
 * Pins configured as inverted will display the oppposite state in the input
 * register.
 */
typedef enum TCA95x5_PIN_POLARITY {
    TCA95x5_NORMAL = 0,
    TCA95x5_INVERTED = 1,
} tca95x5_pin_polarity_t;

/**
 * Pin access for each pin in a port.
 * Each port has 8 pins.
 */
typedef union {
    struct {
        uint8_t p0 : 1;
        uint8_t p1 : 1;
        uint8_t p2 : 1;
        uint8_t p3 : 1;
        uint8_t p4 : 1;
        uint8_t p5 : 1;
        uint8_t p6 : 1;
        uint8_t p7 : 1;
    };
    uint8_t raw;
} tca95x5_port_t;

/**
 * Port/pin access for all I/O
 */
typedef union {
    struct {
        tca95x5_port_t port0;
        tca95x5_port_t port1;
    };
    uint16_t raw;
} tca95x5_register_t;

/**
 * Configuration of each port
 */
typedef struct {
    tca95x5_register_t mode;
    tca95x5_register_t polarity;
    tca95x5_register_t output_state;
} tca95x5_config_t;

///////////////////////////////////////////////////////////////////////////////

class TCA95x5 {
   public:
    tca95x5_config_t config;

    TCA95x5();
    void reset_config();
    void begin();

    void apply_config();
    void apply_mode_config();
    void apply_state_config();
    void apply_polarity_config();
    tca95x5_register_t get_pin_states();
    tca95x5_config_t get_active_config();
    void set_address(bool a0, bool a1, bool a2);
    void set_pin_mode(size_t pin_id, tca95x5_pin_mode_t mode);
    void set_pin_output_state(size_t pin_id, tca95x5_pin_output_state_t state);

   private:
    uint8_t device_address = 0x20;

    bool read(uint8_t *output, tca95x5_reg_address_t address, uint8_t length = 2);
    bool write(uint8_t *input, tca95x5_reg_address_t address, uint8_t length = 2);
};
