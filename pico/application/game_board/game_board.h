#pragma once

#include "ws2812.h"
#include <array>
#include <cstddef>
#include <cstdint>

template<size_t N>
class GameBoard
{
public:
    GameBoard()
    {
    }

    constexpr size_t size()
    {
        return N;
    }

    ws2812::rgb_t *led_mem()
    {
        return leds.data();
    }

    void color(int n, uint8_t r, uint8_t g, uint8_t b)
    {
        leds[n].r = r;
        leds[n].g = g;
        leds[n].b = b;
    }
private:
    std::array<bool, N> pressed{};
    std::array<ws2812::rgb_t, N> leds{};
};
