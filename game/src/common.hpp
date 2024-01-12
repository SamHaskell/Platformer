#pragma once

#include "maths/maths.hpp"
#include "maths/vector.hpp"

#include <SFML/Graphics.hpp>

struct Grid
{
    u32 CellWidth;
    u32 CellHeight;

    inline Vec2 WorldFromGrid(const Vec2 &position) 
    {
        return Vec2{position.x * 32.0f, position.y * 32.0f};
    }

    inline Vec2 GridFromWorld(const Vec2 &position) 
    {
        return Vec2{(f32)std::floor(position.x / 32.0f),
                    (f32)std::floor(position.y / 32.0f)};
    }

    inline Vec2 AlignToGrid(const Vec2& position)
    {
        return WorldFromGrid(GridFromWorld(position));
    }
};

struct TileData
{
    std::string Name;
    std::string TextureSource;
    u32 Width;
    u32 Height;
    i32 OffsetX;
    i32 OffsetY;
};

Vec2 ScreenToWorld(Vec2 screenPos, sf::View& camera, sf::RenderWindow& window);
Vec2 WorldToScreen(Vec2 worldPos, sf::View& camera, sf::RenderWindow& window);