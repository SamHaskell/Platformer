#pragma once

#include "maths/vector.hpp"

/*
    r^2 = (x - x0)^2 + (y - y0)^2
*/
struct Circle
{
    f32 r;
    Vec2 origin;
};

struct IRect
{
    i32 x;
    i32 y;
    i32 w;
    i32 h;
};

struct AABB
{
    f32 x;
    f32 y;
    f32 w;
    f32 h;
};

struct Ray
{
    Vec2 Origin;
    Vec2 Direction;

    Ray(Vec2 origin, Vec2 direction)
        : Origin(origin), Direction(direction) 
    {
        Direction.NormaliseInPlace();
    }
};

struct RaycastHit
{
    Vec2 Point;
    Vec2 Normal;
    f32 Distance;
};