#include "core/scene.hpp"

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