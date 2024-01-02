#include "physics/collisions.hpp"

bool Physics2D::OverlapCircle(Circle a, Circle b)
{
    return Vec2::SqrMagnitude(a.origin - b.origin) < ((a.r + b.r) * (a.r + b.r));
}

bool Physics2D::OverlapAABB(AABB a, AABB b)
{
    bool colliding = false;
    colliding |= (b.w + a.w) / 2.0f > fabsf(b.x - a.x);
    colliding |= (b.h + a.h) / 2.0f > fabsf(b.y - a.y);
    return colliding;
}

bool Physics2D::OverlapAABB(AABB a, AABB b, AABB &outOverlap)
{
    outOverlap.w = (b.w + a.w) / 2.0f - fabsf(b.x - a.x);
    outOverlap.h = (b.h + a.h) / 2.0f - fabsf(b.y - a.y);
    outOverlap.x = (b.x < a.x) ? (b.x + (b.w / 2.0f)) - (outOverlap.w / 2.0f) : (b.x - (b.w / 2.0f)) + (outOverlap.w / 2.0f);
    outOverlap.y = (b.y < a.y) ? (b.y + (b.h / 2.0f)) - (outOverlap.h / 2.0f) : (b.y - (b.h / 2.0f)) + (outOverlap.h / 2.0f);
    
    bool colliding = false;
    colliding |= outOverlap.w > 0.0f;
    colliding |= outOverlap.h > 0.0f;
    return colliding;
}
