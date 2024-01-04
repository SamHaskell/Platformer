#pragma once

#include "maths/geometry.hpp"

namespace Physics2D {
    bool OverlapCircle(Circle a, Circle b);
    bool OverlapAABB(AABB a, AABB b);
    bool OverlapAABB(AABB a, AABB b, AABB& outOverlap);

    bool PointInAABB(Vec2 point, AABB a);

    Ray RayFromTo(Vec2 from, Vec2 to);
    bool Raycast(Ray ray, AABB target, RaycastHit& outHit, f32 maxDistance = 1.0f);
    bool AABBcast(AABB source, AABB target, Vec2 direction, RaycastHit& outHit, f32 maxDistance = 1.0f);
}