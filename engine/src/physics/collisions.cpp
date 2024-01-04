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
    bool colliding = true;

    

    // TODO: Implement AABB collision detection that yields the overlap rect.

    return colliding;
}

bool Physics2D::PointInAABB(Vec2 point, AABB a)
{
    return point.x > a.x && point.x < a.x + a.w && point.y > a.y && point.y < a.y + a.h;
}

Ray RayFromTo(Vec2 from, Vec2 to)
{
    return Ray(from, to - from);
}

bool Physics2D::Raycast(Ray ray, AABB a, RaycastHit& outHit)
{
    Vec2 targetPos = {a.x, a.y};
    Vec2 tNear = (targetPos - ray.Origin) / ray.Direction;
    Vec2 tFar = (targetPos + Vec2{a.w, a.h} - ray.Origin) / ray.Direction;

    if (tNear.x > tFar.x) { std::swap(tNear.x, tFar.x); }

    if (tNear.y > tFar.y) { std::swap(tNear.y, tFar.y); }

    if (tNear.x > tFar.y || tNear.y > tFar.x) { return false; }

    f32 tHitNear = std::max(tNear.x, tNear.y);
    f32 tHitFar = std::min(tFar.x, tFar.y);

    if (tHitFar < 0.0f) { return false; }

    outHit.Point = ray.Origin + ray.Direction * tHitNear;
    outHit.Distance = tHitNear;

    if (tNear.x > tNear.y)
    {
        if (ray.Direction.x < 0.0f)
        {
            outHit.Normal = {1.0f, 0.0f};
        }
        else 
        {
            outHit.Normal = {-1.0f, 0.0f};
        }
    }
    else if (tNear.x < tNear.y)
    {
        if (ray.Direction.y < 0.0f)
        {
            outHit.Normal = {0.0f, 1.0f};
        }
        else 
        {
            outHit.Normal = {0.0f, -1.0f};
        }
    }

    return true;
}