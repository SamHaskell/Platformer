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

bool Physics2D::Raycast(Ray ray, AABB target, RaycastHit& outHit, f32 maxDistance)
{
    // Grab the near and far intersection times for the rays components.

    Vec2 targetPos = {target.x, target.y};

    Vec2 tNear = (targetPos - ray.Origin) / ray.Direction;
    Vec2 tFar = (targetPos + Vec2{target.w, target.h} - ray.Origin) / ray.Direction;

    if (std::isnan(tNear.x) || std::isnan(tNear.y)) { return false; }
    if (std::isnan(tFar.x) || std::isnan(tFar.y)) { return false; }

    if (tNear.x > tFar.x) { std::swap(tNear.x, tFar.x); }
    if (tNear.y > tFar.y) { std::swap(tNear.y, tFar.y); }

    // Now interpret results.

    if (tNear.x > tFar.y || tNear.y > tFar.x) { return false; }

    f32 tHitNear = std::max(tNear.x, tNear.y);
    f32 tHitFar = std::min(tFar.x, tFar.y);

    if (tHitFar < 0.f) { return false; }

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

    if (Vec2::Dot(ray.Direction, outHit.Normal) > 0.f)
    {
        return false;
    }

    return (tHitNear < maxDistance);
}

bool Physics2D::AABBcast(AABB source, AABB target, Vec2 direction, RaycastHit& outHit, f32 maxDistance)
{
    Ray ray = Ray({source.x + source.w/2.0f, source.y + source.h/2.0f}, direction);
    target.x -= source.w/2.0f;
    target.y -= source.h/2.0f;
    target.w += source.w;
    target.h += source.h;
    return Raycast(ray, target, outHit, maxDistance);
}