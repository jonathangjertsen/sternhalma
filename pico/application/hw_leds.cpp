#include "app_config.h"
#include "ws2812.h"

#include <cstddef>

const static ws2812::rgb_t *g_mem = nullptr;
static size_t g_size = 0;

void leds_update()
{
    ws2812::write(NEOPIXEL_PIO_NO, g_mem, g_size);
}

void leds_init(const ws2812::rgb_t *mem, size_t size)
{
    g_mem  = mem;
    g_size = size;

    ws2812::start(NEOPIXEL_PIO_NO, NEOPIXEL_PIO_SM, NEOPIXEL_PIO_PIN);
    leds_update();
}
