#pragma once

#include "core/defines.hpp"
#include "core/resources.hpp"
#include "maths/vector.hpp"
#include "maths/geometry.hpp"

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

struct Component
{
    bool Exists = false;
};

struct CTransform : Component
{
    Vec2 Position = {0, 0};
    Vec2 Scale = {1, 1};
    f32 Rotation = 0.0f;

    CTransform() = default;
    CTransform(Vec2 position, Vec2 scale, f32 rotation) : Position(position), Scale(scale), Rotation(rotation) {}
};

struct CVelocity : Component
{
    Vec2 Velocity = {0, 0};
    f32 AngularVelocity = 0.0f;

    CVelocity() = default;
    CVelocity(Vec2 velocity, f32 angularVelocity) : Velocity(velocity), AngularVelocity(angularVelocity) {}
};

struct CName : Component
{
    std::string Name = "default";

    CName() = default;
    CName(const std::string &name) : Name(name) {}
};

struct CLifetime : Component
{
    f64 Duration = 1.0f;
    f64 Remaining;

    CLifetime() = default;
    CLifetime(f64 duration) : Duration(duration), Remaining(duration){};
};

struct CActions : Component
{
    bool Up = false;
    bool Left = false;
    bool Right = false;
    bool Down = false;
    bool Primary = false;
    bool Secondary = false;

    CActions() = default;
};

struct CSprite : Component
{
    sf::Sprite Sprite;
    f32 Depth = 0.0f;

    CSprite() = default;
    CSprite(sf::Texture& source, i32 srcx, i32 srcy, i32 srcw, i32 srch, f32 depth) : Depth(depth) {
        Sprite = sf::Sprite(source, sf::IntRect(
            srcx, srcy,
            srcw, srch
        ));

        Sprite.setOrigin(
            srcw / 2, 
            srch
        );
    };
};

struct CTile : Component
{
    std::string Name = "default";
    std::string TextureSource = "default";
    u32 Width = 0;
    u32 Height = 0;
    u32 OffsetX = 0;
    u32 OffsetY = 0;
    bool SerializeCollider = false;

    CTile() = default;
    CTile(const std::string& name, const std::string& textureSource, u32 width, u32 height, u32 offsetX, u32 offsetY, bool serializeCollider = false) :
        Name(name), TextureSource(textureSource), Width(width), Height(height), OffsetX(offsetX), OffsetY(offsetY), SerializeCollider(serializeCollider) {}
};

struct CSpriteAnimator : Component
{
    Animation AnimationSource = {};
    bool IsFinished = false;
    bool IsLooping = true;
    u32 CurrentFrame = 0;
    f64 TimeAccumulator = 0.0;

    CSpriteAnimator() = default;
    CSpriteAnimator(Animation anim, bool loop = true) : AnimationSource(anim), IsLooping(loop) 
    {
        CurrentFrame = 0;
        TimeAccumulator = 0.0;
    }

    void SetAnimation(Animation anim, bool loop = true) 
    {
        if (anim == AnimationSource) return;
        AnimationSource = anim;
        IsFinished = false;
        IsLooping = loop;
        CurrentFrame = 0;
        TimeAccumulator = 0.0;
    }
};

struct CPlayerActions : Component
{
    bool Up = false;
    bool Down = false;
    bool Left = false;
    bool Right = false;
    bool Jump = false;

    CPlayerActions() = default;
};

struct CGravity : Component
{
    f32 Gravity = 100.0f;

    CGravity() = default;
    CGravity(f32 gravity) : Gravity(gravity) {}
};

struct CPlayerController : Component
{
    f32 MoveSpeed = 200.0f;
    f32 JumpSpeed = 250.0f;
    bool IsGrounded = false;

    CPlayerController() = default;
    CPlayerController(f32 moveSpeed, f32 jumpSpeed) : MoveSpeed(moveSpeed), JumpSpeed(jumpSpeed) {}
};

struct CBoxCollider : Component
{
    Vec2 Size = {1.0, 1.0};

    CBoxCollider() = default;
    CBoxCollider(Vec2 size) : Size(size) {}
};

struct CButton : Component
{
    sf::Text TextSprite;

    bool IsDown = false;
    bool IsHovering = false;

    std::function<void()> OnPress = [](){};
    std::function<void()> OnRelease = [](){};
    std::function<void()> OnHoverEnter = [](){};
    std::function<void()> OnHoverExit = [](){};

    CButton() = default;
    CButton(const std::string& text,
            sf::Font& font,
            f32 fontSize,
            sf::Color fontColor    
        )
    {
        TextSprite = sf::Text(text, font, fontSize);
        TextSprite.setFillColor(fontColor);
        TextSprite.setOutlineColor(fontColor);
    }
};