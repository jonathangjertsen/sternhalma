/**
 * Onboard LED control for Pi Pico W
 */
#include "onboard_led.h"

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

void onboard_led_init()
{
    if (!cyw43_is_initialized(&cyw43_state))
    {
        cyw43_arch_init();
    }
}

void onboard_led_write(bool value)
{
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, value);
}
