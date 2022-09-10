#include "app_config.h"

#include "ws2812.h"
#include "onboard_led.h"

#include "pico/stdlib.h"

#define NON 0
#define LOW 1
#define MID 4
#define HI_ 16

static const ws2812::rgb_t leds[9] = {
    ws2812::rgb_t{ LOW, NON, NON },
    ws2812::rgb_t{ NON, LOW, NON },
    ws2812::rgb_t{ NON, NON, LOW },
    ws2812::rgb_t{ MID, NON, NON },
    ws2812::rgb_t{ NON, MID, NON },
    ws2812::rgb_t{ NON, NON, MID },
    ws2812::rgb_t{ HI_, NON, NON },
    ws2812::rgb_t{ NON, HI_, NON },
    ws2812::rgb_t{ NON, NON, HI_ },
};

int main(void)
{
    ws2812::start(NEOPIXEL_PIO_NO, NEOPIXEL_PIO_SM, NEOPIXEL_PIO_PIN);
    ws2812::write(NEOPIXEL_PIO_NO, leds, 9);
    onboard_led_init();
    while (1)
    {
        onboard_led_write(1);
        sleep_ms(250);
        onboard_led_write(0);
        sleep_ms(250);
    }
}
