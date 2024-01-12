#include "maths/maths.hpp"
#include "maths/vector.hpp"

f32 Vec2::SqrMagnitude(const Vec2 &v)
{
    return (v.x * v.x) + (v.y * v.y);
}

f32 Vec2::Magnitude(const Vec2 &v)
{
    if (SqrMagnitude(v) == 0.0f)
    {
        return 0.0f;
    } 
    else 
    {
        return sqrtf(SqrMagnitude(v));
    }
}

Vec2 Vec2::Normalised(const Vec2 &v)
{
    f32 normalisation = 1.0f / Magnitude(v);
    if (normalisation == 0.0f)
    {
        return {0.0f, 0.0f};
    } 
    else 
    {
        return {v.x * normalisation, v.y * normalisation};
    }
}

f32 Vec2::Dot(const Vec2 &u, const Vec2 &v)
{
    return (u.x * v.x) + (u.y * v.y);
}

Vec2 Vec2::Hadamard(const Vec2 &u, const Vec2 &v)
{
    return {u.x * v.x, u.y * v.y};
}

Vec2 Vec2::Lerp(const Vec2 &u, const Vec2 &v, f32 t)
{
    return {Maths::Lerp(u.x, v.x, t), Maths::Lerp(u.y, v.y, t)};
}

Vec2 Vec2::LerpClamped(const Vec2 &u, const Vec2 &v, f32 t)
{
    return {Maths::LerpClamped(u.x, v.x, t), Maths::LerpClamped(u.y, v.y, t)};
}