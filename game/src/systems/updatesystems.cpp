#include "updatesystems.hpp"

#include "core/entity.hpp"
#include "maths/maths.hpp"

namespace Systems {
    void UpdateGravity(World& world, f64 dt)
    {
        for (auto e : world.GetEntities())
        {
            if (e->HasComponent<CGravity>() && e->HasComponent<CVelocity>())
            {
                auto &vel = e->GetComponent<CVelocity>();
                vel.Velocity.y -= e->GetComponent<CGravity>().Gravity * dt;
            }
        }
    }

    void UpdatePositions(World& world, f64 dt)
    {
        for (auto e : world.GetEntities())
        {
            if (e->HasComponent<CVelocity>() && e->HasComponent<CTransform>())
            {
                auto &tf = e->GetComponent<CTransform>();
                auto &vel = e->GetComponent<CVelocity>();
                tf.Position += vel.Velocity * dt;
                tf.Rotation += vel.AngularVelocity * dt;
            }
        }
    }

    void UpdateAnimations(World& world, f64 dt)
    {
        for (auto e : world.GetEntities())
        {
            if (e->HasComponent<CSpriteAnimator>())
            {
                auto &anim = e->GetComponent<CSpriteAnimator>();
                anim.TimeAccumulator += dt;

                while (anim.TimeAccumulator > anim.AnimationSource.FrameTime)
                {
                    anim.TimeAccumulator -= anim.AnimationSource.FrameTime;
                    anim.CurrentFrame++;
                }

                if (anim.CurrentFrame == anim.AnimationSource.FrameCount)
                {
                    anim.IsFinished = true;
                    if (anim.IsLooping)
                    {
                        anim.CurrentFrame = 0;
                    }
                    else
                    {
                        anim.CurrentFrame = anim.AnimationSource.FrameCount - 1;
                    }
                }
                else
                {
                    anim.IsFinished = false;
                }

                if (e->HasComponent<CSprite>())
                {
                    auto &sprite = e->GetComponent<CSprite>();

                    sprite.Sprite.setTexture(
                        ResourceManager::GetInstance().GetTexture(anim.AnimationSource.TextureSourceName));

                    sprite.Sprite.setTextureRect(sf::IntRect(
                        anim.AnimationSource.OffsetX + (anim.AnimationSource.FrameWidth * anim.CurrentFrame),
                        anim.AnimationSource.OffsetY,
                        anim.AnimationSource.FrameWidth,
                        anim.AnimationSource.FrameHeight));
                }
            }
        }
    }

    void UpdateCamera(World& world, f64 dt, sf::View& camera, CameraParams& cameraParams)
    {
        auto player = world.GetEntitiesWithTag("player").front();

        if (!player)
        {
            return;
        }

        if (player->HasComponent<CTransform>())
        {
            Vec2 playerPos = player->GetComponent<CTransform>().Position;
            cameraParams.CurrentTarget = playerPos;

            cameraParams.CurrentPosition = Vec2{
                Maths::Lerp(cameraParams.CurrentPosition.x, cameraParams.CurrentTarget.x, (1.0f / cameraParams.DampTimeX) * dt),
                Maths::Lerp(cameraParams.CurrentPosition.y, cameraParams.CurrentTarget.y, (1.0f / cameraParams.DampTimeY) * dt)
            };

            camera.setSize(cameraParams.FrameWidth, cameraParams.FrameHeight);

            camera.setCenter(
                (i32) cameraParams.CurrentPosition.x, 
                (i32)(cameraParams.FrameHeight - cameraParams.CurrentPosition.y)
            );
        } 
    }

    void UpdateTileSprites(World& world, f64 dt)
    {
        for (auto e: world.GetEntitiesWithTag("tile"))
        {
            if (e->HasComponent<CTile>() && e->HasComponent<CSprite>() && e->HasComponent<CBoxCollider>())
            {
                auto &tile = e->GetComponent<CTile>();
                auto &sprite = e->GetComponent<CSprite>();
                auto &collider = e->GetComponent<CBoxCollider>();

                sprite.Sprite.setTexture(
                    ResourceManager::GetInstance().GetTexture(tile.TextureSource));

                sprite.Sprite.setTextureRect(sf::IntRect(
                    tile.OffsetX,
                    tile.OffsetY,
                    tile.Width,
                    tile.Height));

                collider.Size = Vec2{ (f32) tile.Width, (f32) tile.Height };
            }
        }

        for (auto e: world.GetEntitiesWithTag("decoration"))
        {
            if (e->HasComponent<CTile>() && e->HasComponent<CSprite>())
            {
                auto &tile = e->GetComponent<CTile>();
                auto &sprite = e->GetComponent<CSprite>();

                sprite.Sprite.setTexture(
                    ResourceManager::GetInstance().GetTexture(tile.TextureSource));

                sprite.Sprite.setTextureRect(sf::IntRect(
                    tile.OffsetX,
                    tile.OffsetY,
                    tile.Width,
                    tile.Height));
            }
        }
    }

    void UpdatePlayerMovement(std::shared_ptr<Entity> player, f64 dt)
    {
        if (!player->HasComponent<CPlayerActions>())
        {
            return;
        }

        if (!player->HasComponent<CVelocity>())
        {
            return;
        }

        if (!player->HasComponent<CPlayerController>())
        {
            return;
        }

        auto &actions = player->GetComponent<CPlayerActions>();
        auto &vel = player->GetComponent<CVelocity>();
        auto &controller = player->GetComponent<CPlayerController>();

        vel.Velocity.x = 0.0f;

        if (actions.Jump && controller.IsGrounded)
        {
            vel.Velocity.y = controller.JumpSpeed;
        } else if (!actions.Jump)
        {
            vel.Velocity.y = Maths::Clamp(vel.Velocity.y, vel.Velocity.y, controller.JumpSpeed * 0.5f);
        }

        f32 moveSpeed = controller.MoveSpeed;
        if (!controller.IsGrounded)
        {
            moveSpeed *= 0.8f;
        }

        if (actions.Left)
        {
            vel.Velocity.x -= moveSpeed;
        }

        if (actions.Right)
        {
            vel.Velocity.x += moveSpeed;
        }
    }

    void UpdatePlayerAnimationState(std::shared_ptr<Entity> player, f64 dt)
    {
        if (!player->HasComponent<CPlayerActions>())
        {
            return;
        }

        auto &actions = player->GetComponent<CPlayerActions>();

        if (player->HasComponent<CTransform>() && player->HasComponent<CSpriteAnimator>())
        {
            auto &anim = player->GetComponent<CSpriteAnimator>();
            auto &tf = player->GetComponent<CTransform>();

            if (player->HasComponent<CVelocity>() && fabsf(player->GetComponent<CVelocity>().Velocity.y) > 10.0f)
            {
                auto &vel = player->GetComponent<CVelocity>();
                if (vel.Velocity.y > 1.0f)
                {
                    anim.SetAnimation(ResourceManager::GetInstance().GetAnimation("anim_player_rise"));
                }
                else if (vel.Velocity.y < 1.0f)
                {
                    anim.SetAnimation(ResourceManager::GetInstance().GetAnimation("anim_player_fall"));
                }
            }
            else if (actions.Left || actions.Right)
            {
                anim.SetAnimation(ResourceManager::GetInstance().GetAnimation("anim_player_run"));
            }
            else
            {
                anim.SetAnimation(ResourceManager::GetInstance().GetAnimation("anim_player_idle"));
            }

            if (actions.Left)
            {
                tf.Scale.x = -1.0f * fabsf(tf.Scale.x);
            }
            else if (actions.Right)
            {
                tf.Scale.x = 1.0f * fabsf(tf.Scale.x);
            }
        }
    }
    
}