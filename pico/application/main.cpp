#include "app_config.h"

#include "game_board.h"
#include "ws2812.h"
#include "mcp23017.h"
#include "onboard_led.h"

#include "pico/stdlib.h"

#define NON 0
#define LOW 1
#define MID 4
#define HI_ 16

struct slot_t
{
    bool on;
    ws2812::rgb_t color;
};

static GameBoard<9> board{};

static Mcp23017 mcps[] = {
    Mcp23017{BUTTON_MATRIX_I2C_PERIPH, BUTTON_MATRIX_I2C_ADDR_0},
};

void leds_update()
{
    ws2812::write(NEOPIXEL_PIO_NO, board.led_mem(), 9);
}

void leds_init()
{
    ws2812::start(NEOPIXEL_PIO_NO, NEOPIXEL_PIO_SM, NEOPIXEL_PIO_PIN);
    leds_update();
}

void i2c_init_app(void)
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

int main(void)
{
    leds_init();
    i2c_init_app();
    for (const auto &mcp : mcps)
    {
        init_mcp(mcp);
    }
    onboard_led_init();
    onboard_led_write(1);
    while (1)
    {
        for (auto &mcp : mcps)
        {
            uint32_t keys = keyscan(mcp, 3, 3, 1);
            for (int pin = 0; pin < 9; pin++)
            {
                if ((keys & (1 << pin)) == 0)
                {
                    board.color(pin, HI_, MID, MID);
                }
                else
                {
                    board.color(pin, LOW, MID, LOW);
                }
            }
        }
        leds_update();
    }
}
