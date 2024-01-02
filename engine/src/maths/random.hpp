#pragma once

#include "core/hash.hpp"

#include "maths/maths.hpp"
#include "maths/vector.hpp"

namespace Random
{
    void SetGlobalSeed(u32 seed);

    f32 F32();
    f32 F32(f32 min, f32 max);
    u32 U32();
    u32 U32(u32 min, u32 max);
    u64 U64();
    u64 U64(u32 min, u32 max);

    inline f32 F32(u32 &seed)
    {
        seed = Utils::hash_pcg(seed);
        return (f32)seed / (f32)UINT32_MAX;
    }

    inline f32 F32(u32 &seed, f32 min, f32 max)
    {
        return Maths::Lerp(min, max, F32(seed));
    }

    inline u32 U32(u32 &seed)
    {
        seed = Utils::hash_pcg(seed);
        return seed;
    }

    inline u32 U32(u32 &seed, u32 min, u32 max)
    {
        return min + (U32(seed) % (max - min + 1));
    }

    inline u64 U64(u32 &seed)
    {
        u32 bottom = Utils::hash_pcg(seed);
        u32 top = Utils::hash_pcg(bottom);
        seed = top;
        return ((u64)top << 32) & (u64)bottom;
    }

    inline u64 U64(u32 &seed, u64 min, u64 max)
    {
        return min + (U64(seed) % (max - min + 1));
    }
};