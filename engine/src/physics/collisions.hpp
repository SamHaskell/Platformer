#pragma once

#include "maths/geometry.hpp"

/*
    For now collision tests just return a boolean.
    In future they should return a contact struct.
*/

namespace Physics2D {
    bool OverlapCircle(Circle a, Circle b);
    bool OverlapAABB(AABB a, AABB b);
    bool OverlapAABB(AABB a, AABB b, AABB& outOverlap);
}