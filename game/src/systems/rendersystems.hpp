#pragma once

#include "common.hpp"
#include "core/scene.hpp"

namespace Systems {
    void RenderSprites(sf::RenderWindow* window, World& world, sf::View& camera, CameraParams& cameraParams);
    void RenderButtons(sf::RenderWindow* window, World& world, sf::View& camera, CameraParams& cameraParams);
    void RenderBackground(sf::RenderWindow* window);
    
    void EditorRenderSelectedTileSprite(sf::RenderWindow* window, World& world, sf::View& camera, CameraParams& cameraParams, TileData& currentSelectedTile, sf::Sprite& currentSelectedTileSprite, Vec2 mousePos);
}