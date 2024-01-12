#pragma once

#include <string>

enum class ActionType
{
    Begin,
    End
};

struct Action
{
    std::string Name;
    ActionType Type;
    union 
    {
        Vec2 Position;
    };
};