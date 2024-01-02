#pragma once

#include "maths/maths.hpp"
#include "maths/random.hpp"

using Color = Vec4;

namespace ColorUtils {
    Color RandomHue(f32 hueMin, f32 hueMax, f32 saturation, f32 lightness, f32 alpha);
    Color HSL2RGB(const Color& hsl);
}