#include "physics/collisions.hpp"

bool Physics2D::OverlapCircle(Circle a, Circle b)
{
    return Vec2::SqrMagnitude(a.origin - b.origin) < ((a.r + b.r) * (a.r + b.r));
}

bool Physics2D::OverlapAABB(AABB a, AABB b)
{
    bool colliding = true;

    // Simpler AABB collision check that does not yield any collision data.

    colliding &= (a.x < b.x + b.w && a.x + a.w > b.x) || (a.x + a.w > b.x && a.x < b.x + b.w);
    colliding &= (a.y < b.y + b.h && a.y + a.h > b.y) || (a.y + a.h > b.y && a.y < b.y + b.h);

    return colliding;
}

bool Physics2D::OverlapAABB(AABB a, AABB b, AABB &outOverlap)
{   
    bool colliding = false;

    // TODO: Implement AABB collision detection that yields the overlap rect.

    return colliding;
}
