#pragma once

#include "common.hpp"
#include "core/scene.hpp"

namespace Systems {
    void DebugRenderColliders(sf::RenderWindow *window, World& world, sf::View& camera, CameraParams& cameraParams);
    void DebugRenderCamera(sf::RenderWindow *window, sf::View& camera, CameraParams& cameraParams);
    void DebugRenderWorldGrid(sf::RenderWindow *window, const Grid& grid, sf::View& camera, CameraParams& cameraParams);
};