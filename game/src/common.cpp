#include "common.hpp"

Vec2 ScreenToWorld(Vec2 screenPos, sf::View& camera, sf::RenderWindow& window)
{
    Vec2 worldPos;
    Vec2 windowSize = {(f32)window.getSize().x, (f32)window.getSize().y};
    sf::Vector2f cameraPos = camera.getCenter();
    sf::Vector2f cameraSize = camera.getSize();

    worldPos.x = ((screenPos.x / windowSize.x) - 0.5f) * cameraSize.x + cameraPos.x;
    worldPos.y = (1.5f - (screenPos.y / windowSize.y)) * cameraSize.y - cameraPos.y;
    return worldPos;
}

Vec2 WorldToScreen(Vec2 worldPos, sf::View& camera, sf::RenderWindow& window)
{
    Vec2 screenPos;
    Vec2 windowSize = {(f32)window.getSize().x, (f32)window.getSize().y};
    sf::Vector2f cameraPos = camera.getCenter();
    sf::Vector2f cameraSize = camera.getSize();

    screenPos.x = ((worldPos.x - cameraPos.x) / cameraSize.x + 0.5f) * windowSize.x;
    screenPos.y = -((worldPos.y + cameraPos.y) / cameraSize.y - 1.5f) * windowSize.y;
    return screenPos;
}