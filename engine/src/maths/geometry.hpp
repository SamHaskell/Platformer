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

/*
    y = mx + c
*/
struct Line2D
{
    f32 m;
    f32 c;
};