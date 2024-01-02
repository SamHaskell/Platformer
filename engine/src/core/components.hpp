#pragma once

#include "core/defines.hpp"
#include "core/resources.hpp"
#include "maths/vector.hpp"
#include "maths/geometry.hpp"

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

struct CTransform
{
    Vec2 Position = {0, 0};
    Vec2 Scale = {1, 1};
    f32 Rotation = 0.0f;

    CTransform(Vec2 position, Vec2 scale, f32 rotation) : Position(position), Scale(scale), Rotation(rotation) {}
};

struct CVelocity
{
    Vec2 Velocity = {0, 0};
    f32 AngularVelocity = 0.0f;

    CVelocity(Vec2 velocity, f32 angularVelocity) : Velocity(velocity), AngularVelocity(angularVelocity) {}
};

struct CName
{
    std::string Name = "default";

    CName(const std::string &name) : Name(name) {}
};

struct CLifetime
{
    f64 Duration = 1.0f;
    f64 Remaining;

    CLifetime(f64 duration) : Duration(duration), Remaining(duration){};
};

struct CActions
{
    bool Up = false;
    bool Left = false;
    bool Right = false;
    bool Down = false;
    bool Primary = false;
    bool Secondary = false;

    CActions(){};
};

struct CSprite
{
    sf::Sprite Sprite;

    CSprite(sf::Texture& source, i32 srcx, i32 srcy, i32 srcw, i32 srch) {
        Sprite = sf::Sprite(source, sf::IntRect(
            srcx, srcy,
            srcw, srch
        ));

        Sprite.setOrigin(
            static_cast<f32>(srcw) / 2.0f, 
            static_cast<f32>(srch)
        );
    };
};

struct CSpriteAnimator
{
    Animation AnimationSource;
    u32 CurrentFrame;
    f64 TimeAccumulator;

    CSpriteAnimator(Animation anim) : AnimationSource(anim) {
        CurrentFrame = 0;
        TimeAccumulator = 0.0;
    }
};

struct CPlayerActions
{
    bool Up = false;
    bool Down = false;
    bool Left = false;
    bool Right = false;
    bool Jump = false;
};