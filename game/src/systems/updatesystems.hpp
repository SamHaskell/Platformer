#pragma once

#include "core/scene.hpp"

namespace Systems {
    void UpdateGravity(World& world, f64 dt);
    void UpdatePositions(World& world, f64 dt);
}