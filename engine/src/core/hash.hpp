#pragma once

#include "core/defines.hpp"
#include <vector>
#include <functional>

namespace Utils
{
    inline u32 hash_pcg(u32 in)
    {
        u32 state = in * 747796405u + 2891336453u;
        u32 word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
        return (word >> 22u) ^ word;
    }
};