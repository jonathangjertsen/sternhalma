#pragma once

#include <stdint.h>
#include <stddef.h>

namespace ws2812
{

struct rgb_t
{
    uint8_t r;
    uint8_t g;
    uint8_t b;

    uint32_t encode() const
    {
        uint32_t u32 = ((uint32_t) (r) << 8) | ((uint32_t) (g) << 16) | (uint32_t) (b);
        return u32 << 8u;
    }
};

void start(int pio_no, int sm, int pin);
void write(int pio_no, const rgb_t *rgb, size_t n);

}
