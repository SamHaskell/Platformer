#pragma once

#include "core/defines.hpp"

#include <math.h>

struct IRect2D
{
    i32 x;
    i32 y;
    i32 w;
    i32 h;
};

namespace Maths
{
    inline f32 Clamp(f32 x, f32 min, f32 max)
    {
        return (x < min) ? min : (x > max) ? max
                                           : x;
    }

    inline f64 Clamp(f64 x, f64 min, f64 max)
    {
        return (x < min) ? min : (x > max) ? max
                                           : x;
    }

    inline f32 Lerp(f32 a, f32 b, f32 t)
    {
        return a + (b - a) * t;
    }

    inline f32 LerpClamped(f32 a, f32 b, f32 t)
    {
        return a + (b - a) * Clamp(t, 0.0f, 1.0f);
    }

    inline f32 Unlerp(f32 a, f32 b, f32 y)
    {
        return (y - a) / (b - a);
    }
};