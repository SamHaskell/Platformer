#pragma once

#include "core/scene.hpp"

namespace Systems {
    void DebugRenderColliders(sf::RenderWindow *window, World& world, sf::View& camera, CameraParams& cameraParams);
};