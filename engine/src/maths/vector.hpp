#pragma once

#include "core/defines.hpp"

struct Vec2
{
    f32 x;
    f32 y;

    bool operator==(const Vec2 &other) const { return (x == other.x) && (y == other.y); }
    void operator*=(const f32 val)
    {
        x *= val;
        y *= val;
    }
    void operator*=(const Vec2 &v)
    {
        x *= v.x;
        y *= v.y;
    }
    Vec2 operator*(const f32 val) const { return {x * val, y * val}; }
    Vec2 operator*(const Vec2 &v) const { return {x * v.x, y * v.y}; }
    void operator+=(const f32 val)
    {
        x += val;
        y += val;
    }
    void operator+=(const Vec2 &v)
    {
        x += v.x;
        y += v.y;
    }
    Vec2 operator+(const f32 val) const { return {x + val, y + val}; }
    Vec2 operator+(const Vec2 &v) const { return {x + v.x, y + v.y}; }
    void operator-=(const f32 val)
    {
        x -= val;
        y -= val;
    }
    void operator-=(const Vec2 &v)
    {
        x -= v.x;
        y -= v.y;
    }
    Vec2 operator-(const f32 val) const { return {x - val, y - val}; }
    Vec2 operator-(const Vec2 &v) const { return {x - v.x, y - v.y}; }

    static Vec2 Up() { return {0.0, 1.0}; }
    static Vec2 Down() { return {0.0, -1.0}; }
    static Vec2 Right() { return {1.0, 0.0}; }
    static Vec2 Left() { return {-1.0, 0.0}; }

    static f32 SqrMagnitude(const Vec2 &v);
    static f32 Magnitude(const Vec2 &v);
    static Vec2 Normalised(const Vec2 &v);
    static f32 Dot(const Vec2 &u, const Vec2 &v);
    static Vec2 Hadamard(const Vec2 &u, const Vec2 &v);
    static Vec2 Lerp(const Vec2 &u, const Vec2 &v, f32 t);
    static Vec2 LerpClamped(const Vec2 &u, const Vec2 &v, f32 t);

    inline Vec2 &Translate(const Vec2 &vec)
    {
        x += vec.x;
        y += vec.y;
        return *this;
    }

    inline Vec2 &Scale(f32 scale)
    {
        x *= scale;
        y *= scale;
        return *this;
    }

    inline Vec2 &AddScaledVector(const Vec2 &vec, f32 scale)
    {
        x += vec.x * scale;
        y += vec.y * scale;
        return *this;
    }

    inline Vec2 &HadamardInPlace(const Vec2 &other)
    {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    inline Vec2 &NormaliseInPlace()
    {
        f32 mag = Magnitude(*this);
        if (mag == 0)
        {
            return *this;
        }

        f32 inv = 1.0f / mag;
        x *= inv;
        y *= inv;
        return *this;
    }
};

struct Vec4 {
    union {
        struct {
            f32 x, y, z, w;
        };
        struct {
            f32 r, g, b, a;
        };
        f32 values[4];
    };
};

inline Vec2 operator-(const Vec2 &v) { return {-v.x, -v.y}; }