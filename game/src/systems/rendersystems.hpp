#pragma once

#include "core/scene.hpp"

struct TileData
{
    std::string Name;
    std::string TextureSource;
    u32 Width;
    u32 Height;
    i32 OffsetX;
    i32 OffsetY;
};

namespace Systems {
    void RenderSprites(sf::RenderWindow* window, World& world, sf::View& camera, CameraParams& cameraParams);
    void RenderButtons(sf::RenderWindow* window, World& world, sf::View& camera, CameraParams& cameraParams);
    void RenderBackground(sf::RenderWindow* window);
    
    void EditorRenderSelectedTileSprite(sf::RenderWindow* window, World& world, sf::View& camera, CameraParams& cameraParams, TileData& currentSelectedTile, sf::Sprite& currentSelectedTileSprite, Vec2 mousePos);
}