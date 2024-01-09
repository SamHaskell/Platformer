#include "debugsystems.hpp"

namespace Systems {
    void DebugRenderColliders(sf::RenderWindow *window, World& world, sf::View& camera, CameraParams& cameraParams)
    {
        window->setView(camera);

        for (auto e : world.GetEntities())
        {
            if (e->HasComponent<CBoxCollider>() && e->HasComponent<CTransform>())
            {
                auto &tf = e->GetComponent<CTransform>();
                auto &collider = e->GetComponent<CBoxCollider>();

                Vec2 pos = tf.Position;
                Vec2 scale = tf.Scale;
                Vec2 size = collider.Size;

                sf::Color col = sf::Color::Red;
                col.a = 255;

                sf::RectangleShape rect(sf::Vector2f(size.x, size.y));
                rect.setFillColor(sf::Color::Transparent);
                rect.setOutlineColor(col);
                rect.setOutlineThickness(0.2f);
                rect.setOrigin(size.x / 2.0f, size.y);
                rect.setPosition((i32)pos.x, cameraParams.FrameHeight - (i32)pos.y);
                rect.setScale(scale.x, scale.y);
                window->draw(rect, sf::BlendAlpha);
            }
        }
    }
}