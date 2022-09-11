#pragma once

#include "ws2812.h"
#include <cstddef>

void leds_update();

void leds_init(const ws2812::rgb_t *mem, size_t size);
