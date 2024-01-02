#pragma once

#include "core/defines.hpp"

#include "core/components.hpp"

#include <string>

class Entity {
    friend class World;

    public:
        inline void Destroy() { m_Alive = false; }

        inline bool IsAlive() { return m_Alive; }
        inline std::string& GetTag() { return m_Tag; }
        inline size_t GetID() { return m_ID; }

        // Components

        std::shared_ptr<CTransform> Transform;
        std::shared_ptr<CVelocity> Velocity;
        std::shared_ptr<CName> Name;
        std::shared_ptr<CLifetime> Lifetime;
        std::shared_ptr<CActions> Actions;
        std::shared_ptr<CSpriteAnimator> Animator;
        std::shared_ptr<CSprite> Sprite;
        std::shared_ptr<CPlayerActions> PlayerActions;

    private:
        Entity(const std::string& tag, size_t id) : m_Tag(tag), m_ID(id) {};

        std::string m_Tag;
        bool m_Alive = true;
        size_t m_ID;
};