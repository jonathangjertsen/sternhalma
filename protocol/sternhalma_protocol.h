#pragma once

namespace sternhalma
{

constexpr int N_SLOTS = 121;
constexpr int N_BYTES = ((N_SLOTS + 7) / 8);

struct sternhalma_inputs_t
{
    uint8_t buttons[N_BYTES];
    uint8_t errors; // Bit0-5: error for each wing
};

struct sternhalma_outputs_t
{
    uint32_t rgb[N_SLOTS];
};

}
