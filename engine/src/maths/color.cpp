#include "color.hpp"

namespace ColorUtils
{
    Color RandomHue(f32 hueMin, f32 hueMax, f32 saturation, f32 lightness, f32 alpha)
    {
        Color hsl;
        
        hsl.x = Random::F32(hueMin, hueMax);
        hsl.y = saturation;
        hsl.z = lightness;
        hsl.w = alpha;

        return HSL2RGB(hsl);
    }

    Color HSL2RGB(const Color& hsl)
    {
        Color rgb;

        f64 C = (1.0f - std::abs(2.0f * hsl.z - 1.0f)) * hsl.y;
        f64 X = C * (1.0f - std::abs(fmod(hsl.x * 6.0f, 2.0f) - 1.0f));
        f64 m = hsl.z - C / 2.0f;

        f64 R = 0, G = 0, B = 0;
        if (0 <= hsl.x && hsl.x < 1.0 / 6.0) {
            R = C;
            G = X;
        } else if (1.0 / 6.0 <= hsl.x && hsl.x < 2.0 / 6.0) {
            R = X;
            G = C;
        } else if (2.0 / 6.0 <= hsl.x && hsl.x < 3.0 / 6.0) {
            G = C;
            B = X;
        } else if (3.0 / 6.0 <= hsl.x && hsl.x < 4.0 / 6.0) {
            G = X;
            B = C;
        } else if (4.0 / 6.0 <= hsl.x && hsl.x < 5.0 / 6.0) {
            R = X;
            B = C;
        } else if (5.0 / 6.0 <= hsl.x && hsl.x <= 1.0) {
            R = C;
            B = X;
        }

        rgb.r = static_cast<f32>((R + m));
        rgb.g = static_cast<f32>((G + m));
        rgb.b = static_cast<f32>((B + m));
        rgb.a = hsl.w;

        return rgb;
    }
}