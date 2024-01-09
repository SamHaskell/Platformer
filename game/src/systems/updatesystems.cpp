#include "updatesystems.hpp"

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
}