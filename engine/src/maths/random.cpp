#include "maths/random.hpp"

#include <time.h>

namespace Random
{
    /*
        NOT SECURE OBVIOUSLY
    */
    static u32 s_GlobalSeed = static_cast<u32>(time(NULL));

    void SetGlobalSeed(u32 seed)
    {
        s_GlobalSeed = seed;
    }

    f32 F32()
    {
        return F32(s_GlobalSeed);
    }

    f32 F32(f32 min, f32 max)
    {
        return F32(s_GlobalSeed, min, max);
    }

    u32 U32()
    {
        return U32(s_GlobalSeed);
    }

    u32 U32(u32 min, u32 max)
    {
        return U32(s_GlobalSeed, min, max);
    }
}