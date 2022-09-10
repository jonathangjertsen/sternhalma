#ifndef ONBOARD_LED_H
#define ONBOARD_LED_H

#ifdef __cplusplus
    extern "C" {
#endif

#include <stdbool.h>

/**
 * Initialize the onboard LED
 */
void onboard_led_init(void);

/**
 * Set the LED to the given value
 */
void onboard_led_write(bool value);

#ifdef __cplusplus
    }
#endif

#endif
