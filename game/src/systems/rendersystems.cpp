#include "rendersystems.hpp"

namespace Systems {
    void RenderSprites(sf::RenderWindow* window, World& world, sf::View& camera, CameraParams& cameraParams)
    {
        window->setView(camera);

        std::vector<CSprite *> sprites;

        for (auto e : world.GetEntities())
        {
            if (e->HasComponent<CSprite>() && e->HasComponent<CTransform>())
            {
                auto &sprite = e->GetComponent<CSprite>();
                auto &tf = e->GetComponent<CTransform>();
                Vec2 pos = tf.Position;
                f32 rot = tf.Rotation;
                Vec2 scale = tf.Scale;

                sprite.Sprite.setPosition((i32)pos.x, (i32)cameraParams.FrameHeight - (i32)pos.y);
                sprite.Sprite.setRotation(rot);
                sprite.Sprite.setScale(scale.x, scale.y);

                sprites.push_back(&sprite);
            }
        }

        std::sort(sprites.begin(), sprites.end(), [](CSprite *a, CSprite *b)
                { return a->Depth < b->Depth; });

        for (auto sprite : sprites)
        {
            window->draw(sprite->Sprite);
        }
    }

    void RenderBackground(sf::RenderWindow* window)
    {
        window->setView(window->getDefaultView());

        // Draw background texture to whole screen

        sf::Texture& tex = ResourceManager::GetInstance().GetTexture("tex_background");

        auto texSize = tex.getSize();
        auto windowSize = window->getSize();

        sf::Sprite sprite(tex);
        sprite.setScale(
            (f32)windowSize.x / (f32)texSize.x,
            (f32)windowSize.y / (f32)texSize.y
        );

        window->draw(sprite);
    }
};