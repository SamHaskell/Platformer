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

    void EditorRenderSelectedTileSprite(sf::RenderWindow* window, World& world, sf::View& camera, CameraParams& cameraParams, TileData& currentSelectedTile, sf::Sprite& currentSelectedTileSprite, Vec2 mousePos)
    {
        window->setView(camera);

        mousePos = Vec2{(f32)std::floor(mousePos.x / 32.0f), (f32)std::floor(mousePos.y / 32.0f)} * 32.0f;
        mousePos.x += 16.0f;

        currentSelectedTileSprite.setScale(2.0f, 2.0f);
        currentSelectedTileSprite.setPosition(mousePos.x, cameraParams.FrameHeight - mousePos.y);
        currentSelectedTileSprite.setOrigin(currentSelectedTile.Width / 2.0f, currentSelectedTile.Height);
        window->draw(currentSelectedTileSprite);   
    }

    void RenderButtons(sf::RenderWindow* window, World& world, sf::View& camera, CameraParams& cameraParams)
    {
        window->setView(camera);

        auto buttons = world.GetEntitiesWithTag("button");

        for (auto e : buttons)
        {
            auto& button = e->GetComponent<CButton>();

            // Draw button sprite.

            auto& coll = e->GetComponent<CBoxCollider>();
            auto& tf = e->GetComponent<CTransform>();

            Vec2 pos = tf.Position;
            pos.y += coll.Size.y / 2.0f;

            // Center text on this position

            sf::Vector2f textSize = button.TextSprite.getLocalBounds().getSize();
            button.TextSprite.setOrigin(textSize.x / 2.0f, coll.Size.y / 2.0f);

            f32 rot = tf.Rotation;
            Vec2 scale = tf.Scale;

            button.TextSprite.setPosition((i32)pos.x, (i32)cameraParams.FrameHeight - (i32)pos.y);
            button.TextSprite.setRotation(rot);
            button.TextSprite.setScale(scale.x, scale.y);

            window->draw(button.TextSprite);
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