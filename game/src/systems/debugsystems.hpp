#pragma once

#include "common.hpp"
#include "core/scene.hpp"

namespace Systems {
    void DebugRenderColliders(sf::RenderWindow *window, World& world, sf::View& camera, CameraParams& cameraParams);
};