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
};