#pragma once

#include "core/defines.hpp"

#include "core/entity.hpp"

#include <vector>
#include <map>

using EntityVector = std::vector<std::shared_ptr<Entity>>;

class World 
{
    public:
        World() = default;
        ~World() = default;

        void Init();
        void Update();

        std::shared_ptr<Entity> AddEntity(const std::string& tag);
        EntityVector& GetEntities();
        EntityVector& GetEntitiesWithTag(const std::string& tag);

        inline u32 GetEntityCount() { return m_Entities.size(); }

    private:
        size_t m_NextID = 0;
        EntityVector m_Entities;
        EntityVector m_EntitiesToAdd;
        std::map<std::string, EntityVector> m_EntityMap;
};