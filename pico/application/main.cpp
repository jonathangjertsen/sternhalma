#include "onboard_led.h"
#include "game_board.h"

#include "app_config.h"
#include "hw_leds.h"
#include "hw_buttons.h"

#include "debouncer.h"
#include "ws2812.h"
#include "mcp23017.h"
#include "debouncer.h"

static ActiveLowDebouncer debouncers[TOTAL_SLOTS];

static GameBoard<TOTAL_SLOTS> board{};
static Mcp23017 mcps[] = {
    Mcp23017{BUTTON_MATRIX_I2C_PERIPH, BUTTON_MATRIX_I2C_ADDR_0},
    Mcp23017{BUTTON_MATRIX_I2C_PERIPH, BUTTON_MATRIX_I2C_ADDR_1},
    Mcp23017{BUTTON_MATRIX_I2C_PERIPH, BUTTON_MATRIX_I2C_ADDR_2},
    Mcp23017{BUTTON_MATRIX_I2C_PERIPH, BUTTON_MATRIX_I2C_ADDR_3},
    Mcp23017{BUTTON_MATRIX_I2C_PERIPH, BUTTON_MATRIX_I2C_ADDR_4},
    Mcp23017{BUTTON_MATRIX_I2C_PERIPH, BUTTON_MATRIX_I2C_ADDR_5},
};

void keyscan_all()
{
    for (int i = 0; i < N_SLICES; i++)
    {
        uint32_t keys = keyscan(mcps[i], 3, 3, 1);
        for (int pin = 0; pin < SLOTS_PER_SLICE; pin++)
        {
            int mapped = i * SLOTS_PER_SLICE + pin;
            bool raw = (keys & (1 << pin)) == 0;
            board.set_pressed(mapped, debouncers[mapped].update(raw));
        }
    }
}

int main(void)
{
    leds_init(board.led_mem(), board.size());
    buttons_init();
    for (const auto &mcp : mcps)
    {
        init_mcp(mcp);
    }
    onboard_led_init();
    onboard_led_write(1);
    while (1)
    {
        keyscan_all();
        board.update_colors();
        leds_update();
    }
}
