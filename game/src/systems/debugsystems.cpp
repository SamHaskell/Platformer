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

    void DebugRenderCamera(sf::RenderWindow *window, sf::View& camera, CameraParams& cameraParams)
    {
        window->setView(camera);

        sf::Color col = sf::Color::Green;
        col.a = 255;

        sf::RectangleShape rect(sf::Vector2f(cameraParams.BoxWidth, cameraParams.BoxHeight));
        
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(col);
        rect.setOutlineThickness(0.2f);
        rect.setOrigin(cameraParams.BoxWidth / 2.0f, cameraParams.BoxHeight / 2.0f);

        rect.setPosition(
            (i32)cameraParams.CurrentPosition.x, 
            cameraParams.FrameHeight - (i32)cameraParams.CurrentPosition.y
        );
        
        window->draw(rect, sf::BlendAlpha);   
    }

    void DebugRenderWorldGrid(sf::RenderWindow *window, const Grid& grid, sf::View& camera, CameraParams& cameraParams)
    {
        window->setView(window->getDefaultView());

        sf::Vector2i cameraSize = (sf::Vector2i)camera.getSize();
        sf::Vector2i cameraPos = (sf::Vector2i)camera.getCenter();

        i32 gridLeft = ((cameraPos.x - cameraSize.x / 2) / 32) * 32 - 32;
        i32 gridRight = ((cameraPos.x + cameraSize.x / 2) / 32) * 32 + 32;

        i32 gridTop = ((cameraSize.y / 2 - cameraPos.y) / 32) * 32 - 32;
        i32 gridBottom =
            ((cameraSize.y + cameraSize.y / 2 - cameraPos.y) / 32) * 32 + 32;

        sf::Color col = sf::Color::Black;
        col.a = 80;

        for (i32 i = gridLeft; i <= gridRight; i += 32) {
            Vec2 screenTop = WorldToScreen({(f32)i, (f32)(gridTop)}, camera, *window);
            Vec2 screenBottom = WorldToScreen({(f32)i, (f32)(gridBottom)}, camera, *window);

            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(screenTop.x, screenTop.y), col),
                sf::Vertex(sf::Vector2f(screenBottom.x, screenBottom.y), col)};
            window->draw(line, 2, sf::Lines);
        }

        for (i32 j = gridTop; j <= gridBottom; j += 32) {
            Vec2 screenLeft = WorldToScreen({(f32)gridLeft, (f32)j}, camera, *window);
            Vec2 screenRight = WorldToScreen({(f32)gridRight, (f32)j}, camera, *window);

            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(screenLeft.x, screenLeft.y), col),
                sf::Vertex(sf::Vector2f(screenRight.x, screenRight.y), col)};
            window->draw(line, 2, sf::Lines);
        }

        // Draw coords in corners of grid cells

        col.a = 255;

        sf::Text text;
        text.setFont(ResourceManager::GetInstance().GetFont("font_noto"));
        text.setCharacterSize(8);
        text.setFillColor(col);

        for (i32 i = gridLeft; i <= gridRight; i += 32) {
            for (i32 j = gridTop; j <= gridBottom; j += 32) {
                Vec2 screenPos = WorldToScreen({(f32)i + 2, (f32)j + 30}, camera, *window);
                text.setString(std::to_string(i / 32) + ", " +
                            std::to_string(j / 32));
                text.setPosition(screenPos.x, screenPos.y);
                window->draw(text);
            }
        }
    }
}