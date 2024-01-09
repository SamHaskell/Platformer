#include "core/scene.hpp"
#include "core/game.hpp"

void Scene::RegisterAction(i32 keycode, std::string name) {
    if (m_ActionMap.find(keycode) != m_ActionMap.end()) {
        NT_ERROR("Could not bind action, action bound to given key already exists.");
        return;
    }

    m_ActionMap.insert(std::make_pair(keycode, name));
}

bool Scene::GetAction(i32 keycode, std::string& name) {
    bool found = m_ActionMap.find(keycode) != m_ActionMap.end();
    if (found) {
        name = m_ActionMap[keycode];
    }
    return found;
}

Vec2 Scene::ScreenToWorld(Vec2 screenPos)
{
    Vec2 worldPos;
    Vec2 windowSize = m_Game->GetWindowSize();
    sf::Vector2f cameraPos = m_Camera.getCenter();
    sf::Vector2f cameraSize = m_Camera.getSize();

    worldPos.x = ((screenPos.x / windowSize.x) - 0.5f) * cameraSize.x + cameraPos.x;
    worldPos.y = (1.5f - (screenPos.y / windowSize.y)) * cameraSize.y - cameraPos.y;
    return worldPos;
}

Vec2 Scene::WorldToScreen(Vec2 worldPos)
{
    Vec2 screenPos;
    Vec2 windowSize = m_Game->GetWindowSize();
    sf::Vector2f cameraPos = m_Camera.getCenter();
    sf::Vector2f cameraSize = m_Camera.getSize();

    screenPos.x = ((worldPos.x - cameraPos.x) / cameraSize.x + 0.5f) * windowSize.x;
    screenPos.y = -((worldPos.y + cameraPos.y) / cameraSize.y - 1.5f) * windowSize.y;
    return screenPos;
}