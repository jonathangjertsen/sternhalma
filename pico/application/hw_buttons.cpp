#include "app_config.h"
#include "hw_buttons.h"

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include <cstddef>

void buttons_init(void)
{
    i2c_init(BUTTON_MATRIX_I2C_PERIPH, BUTTON_MATRIX_I2C_SPEED);
    gpio_set_function(BUTTON_MATRIX_I2C_PIN_SDA, GPIO_FUNC_I2C);
    gpio_set_function(BUTTON_MATRIX_I2C_PIN_SCL, GPIO_FUNC_I2C);
}

void init_mcp(const Mcp23017 &mcp)
{
    mcp.setup(false, true);
    mcp.set_io_direction(0xff00);
    mcp.set_pullup(0xff00);
}

uint32_t keyscan(Mcp23017 &mcp, int columns, int rows, int interval_ms)
{
    // Port A pins are driven
    // Port B pins are read
    uint32_t keys = 0;
    const auto mask = (1 << rows) - 1;
    for (int col = 0; col < columns; col++)
    {
        mcp.set_all_output_bits(0xffff &~ (1 << col));
        sleep_ms(interval_ms);
        mcp.update_input_values();
        uint32_t pins = mcp.get_input_pin_values();
        pins >>= 8;
        pins &= mask;
        pins <<= col * rows;
        keys |= pins;
    }
    return keys;
}
