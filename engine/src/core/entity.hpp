#pragma once

#include "core/defines.hpp"

#include "core/components.hpp"

#include <string>
#include <tuple>

class Entity {
    friend class World;

    using Components = std::tuple
    <
        CTransform,
        CVelocity,
        CName,
        CLifetime,
        CActions,
        CSpriteAnimator,
        CSprite,
        CPlayerActions,
        CGravity,
        CBoxCollider,
        CPlayerController,
        CButton
    >;

    public:
        inline void Destroy() { m_Alive = false; }

        inline bool IsAlive() { return m_Alive; }
        inline std::string& GetTag() { return m_Tag; }
        inline size_t GetID() { return m_ID; }

        template <typename TComponent> TComponent& GetComponent();
        template <typename TComponent, typename... TArgs> TComponent& AddComponent(TArgs&&... args);
        template <typename TComponent> void RemoveComponent();
        template <typename TComponent> bool HasComponent();

    private:
        Entity(const std::string& tag, size_t id) : m_Tag(tag), m_ID(id) {};

        std::string m_Tag;
        bool m_Alive = true;
        size_t m_ID;
        Components m_Components;
};

template <typename TComponent>
TComponent& Entity::GetComponent() {
    return std::get<TComponent>(m_Components);
}

template <typename TComponent> 
void Entity::RemoveComponent()
{
    auto& comp = GetComponent<TComponent>() = TComponent();
    comp.Exists = false;
}

template <typename TComponent, typename... TArgs>
TComponent& Entity::AddComponent(TArgs&&... args) {
    auto& comp = GetComponent<TComponent>();
    comp = TComponent(std::forward<TArgs>(args)...);
    comp.Exists = true;
    return comp;
}

template <typename TComponent>
bool Entity::HasComponent() {
    return GetComponent<TComponent>().Exists;
}