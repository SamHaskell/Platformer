#pragma once

#include "maths/maths.hpp"

struct Grid
{
    u32 CellWidth;
    u32 CellHeight;
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