#pragma once

#include "common.hpp"

#include <functional>
#include <string>
#include <vector>

namespace GUI
{
    void DrawLevelSelector(i32& currentLevelIndex, std::vector<std::string>& levelPaths, std::function<void(const std::string&)> onSelect);

    void DrawTilePainter(bool& paintingEnabled, bool& eraseEnabled, bool& paintColliders, std::map<std::string, TileData>& tileData, TileData& currentTile, sf::Sprite& currentTileSprite);
}