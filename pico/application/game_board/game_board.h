#pragma once

#include "ws2812.h"
#include <array>
#include <cstddef>
#include <cstdint>

#define NON 0
#define LOW 1
#define MID 4
#define HI_ 16

template<int TOTAL_SLOTS>
class GameBoard
{
public:
    GameBoard()
    {
    }

    constexpr size_t size()
    {
        return TOTAL_SLOTS;
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

    void set_pressed(int n, bool pressed)
    {
        if (pressed)
        {
            player_owns[n] = 1 + player;
            player = !player;
        }
    }

    void update_colors()
    {
        for (int i = 0; i < TOTAL_SLOTS; i++)
        {
            switch (player_owns[i])
            {
                case 0:
                    color(i, LOW, LOW, LOW);
                    break;
                case 1:
                    color(i, HI_, MID, MID);
                    break;
                case 2:
                    color(i, MID, MID, HI_);
                    break;
            }
        }
    }
private:
    std::array<int, TOTAL_SLOTS> player_owns{};
    std::array<ws2812::rgb_t, TOTAL_SLOTS> leds{};
    bool player = false;
};
