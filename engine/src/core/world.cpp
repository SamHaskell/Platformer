#include "core/world.hpp"

void World::Init() {
    m_NextID = 0;
    m_Entities.clear();
    m_EntitiesToAdd.clear();
    m_EntityMap.clear();
}

void World::Update() {
    for (auto e : m_EntitiesToAdd) {
        m_Entities.push_back(e);
        m_EntityMap[e->GetTag()].push_back(e);
    }

    m_EntitiesToAdd.clear();

    m_Entities.erase(
        std::remove_if(
            m_Entities.begin(),
            m_Entities.end(),
            [&] (std::shared_ptr<Entity> e) {
                return !e->IsAlive();
            }
        ),
        m_Entities.end()
    );

    for (auto& entities : m_EntityMap) {
    entities.second.erase(
        std::remove_if(
            entities.second.begin(),
            entities.second.end(),
            [&] (std::shared_ptr<Entity> e) {
                return !e->IsAlive();
            }
        ),
        entities.second.end()
    );
    }
}

std::shared_ptr<Entity> World::AddEntity(const std::string& tag) {
    auto e = std::shared_ptr<Entity>(new Entity(tag, m_NextID++));
    m_EntitiesToAdd.push_back(e);
    m_EntityMap[tag].push_back(e);
    return e;
}

EntityVector& World::GetEntities() {
    return m_Entities;
}

EntityVector& World::GetEntitiesWithTag(const std::string& tag) {
    return m_EntityMap[tag];
}
