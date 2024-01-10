#include "physicssystems.hpp"

#include "physics/collisions.hpp"

namespace Systems {
    void PhysicsCheckCollisions(World& world, f64 dt)
    {
        /*
            Swept AABB (interpolated) collision checks for player controller.
            Intensive, but only performed for the player so probably fine.
            If wanted to extend to some generic rigidbody then would certainly need to optimise, e.g. broadphase checks first.

            Currently we check for collisions with all tiles, order hits based on distance and then
            iterate through a second time, running another collision check. This is to ensure
            the player does not get stuck at boundaries between tiles.
        */

        // Check collisions between player and tiles

        auto player = world.GetEntitiesWithTag("player").front();

        if (!player)
        {
            return;
        }

        if (player->HasComponent<CBoxCollider>() && player->HasComponent<CTransform>() && player->HasComponent<CVelocity>())
        {
            auto &playerCollider = player->GetComponent<CBoxCollider>();
            auto &playerTransform = player->GetComponent<CTransform>();
            auto &playerVelocity = player->GetComponent<CVelocity>();

            player->GetComponent<CPlayerController>().IsGrounded = false;

            AABB playerBox = {
                playerTransform.Position.x - (playerCollider.Size.x * fabsf(playerTransform.Scale.x)) / 2.0f,
                playerTransform.Position.y,
                playerCollider.Size.x * fabsf(playerTransform.Scale.x),
                playerCollider.Size.y * fabsf(playerTransform.Scale.y)};

            struct BoxHit
            {
                AABB box;
                RaycastHit hit;
            };

            std::vector<BoxHit> hits;

            for (auto e : world.GetEntitiesWithTag("tile"))
            {
                if (e->HasComponent<CBoxCollider>() && e->HasComponent<CTransform>())
                {
                    auto &collider = e->GetComponent<CBoxCollider>();
                    auto &tf = e->GetComponent<CTransform>();

                    AABB tileBox = {
                        tf.Position.x - (collider.Size.x * fabsf(tf.Scale.x)) / 2.0f,
                        tf.Position.y,
                        collider.Size.x * fabsf(tf.Scale.x),
                        collider.Size.y * fabsf(tf.Scale.x)};

                    RaycastHit hit{};
                    if (Physics2D::AABBcast(
                            playerBox,
                            tileBox,
                            Vec2::Normalised(playerVelocity.Velocity),
                            hit,
                            Vec2::Magnitude(playerVelocity.Velocity) * dt))
                    {
                        BoxHit boxhit = {tileBox, hit};
                        hits.push_back(boxhit);
                    }
                }
            }

            std::sort(hits.begin(), hits.end(), [](BoxHit a, BoxHit b)
                    { return a.hit.Distance < b.hit.Distance; });

            for (auto hit : hits)
            {
                RaycastHit second_hit{};
                if (Physics2D::AABBcast(
                        playerBox,
                        hit.box,
                        Vec2::Normalised(playerVelocity.Velocity),
                        second_hit,
                        Vec2::Magnitude(playerVelocity.Velocity) * dt))
                {
                    f32 dist = Vec2::Magnitude(playerVelocity.Velocity) * dt;
                    f32 distLeft = (1.0f - second_hit.Distance / dist);


                    if (second_hit.Distance < 0.0f)
                    {
                        playerTransform.Position = second_hit.Point + second_hit.Normal * 0.1f;
                        playerTransform.Position.y -= (playerCollider.Size.y * fabsf(playerTransform.Scale.y)) / 2.0f;
                    } else {
                        NT_INFO("distLeft: %.3f", distLeft);
                        playerVelocity.Velocity -= second_hit.Normal * Vec2::Dot(second_hit.Normal, playerVelocity.Velocity) * distLeft;
                    }


                    if (second_hit.Normal.y > 0.0f)
                    {
                        player->GetComponent<CPlayerController>().IsGrounded = true;
                    }
                }
            }
        }
    }
}