#pragma once

#include "common.hpp"
#include "core/scene.hpp"

namespace Systems {
    void UpdateGravity(World& world, f64 dt);
    void UpdatePositions(World& world, f64 dt);
    void UpdateAnimations(World& world, f64 dt);
    void UpdateCamera(World& world, f64 dt, sf::View& camera, CameraParams& cameraParams);

    void UpdateTileSprites(World& world, f64 dt);

    void UpdatePlayerMovement(std::shared_ptr<Entity> player, f64 dt);
    void UpdatePlayerAnimationState(std::shared_ptr<Entity> player, f64 dt);
}