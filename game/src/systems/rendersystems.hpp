#pragma once

#include "core/scene.hpp"

namespace Systems {
    void RenderSprites(sf::RenderWindow* window, World& world, sf::View& camera, CameraParams& cameraParams);
    void RenderBackground(sf::RenderWindow* window);
}