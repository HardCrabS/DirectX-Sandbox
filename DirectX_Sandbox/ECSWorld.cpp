#include "ECSWorld.h"
#include <iostream>

void ECSWorld::Initialize()
{
    m_systems.push_back(std::move(std::make_unique<CameraSystem>()));
    m_systems.push_back(std::move(std::make_unique<RenderSystem>()));

    for (auto& system : m_systems) {
        system->Initialize();
    }
}

void ECSWorld::Update()
{
    for (auto& system : m_systems)
    {
        system->Tick();

        for (auto& it : m_entities)
        {
            Entity* entity = it.second.get();
            if (system->IsEntityHasRequiredComponents(entity)) {
                system->UpdateEntity(entity);
            }
        }
    }
}

Entity* ECSWorld::CreateEntity()
{
    int id;
    if (m_freeEntityIDsPool.size() > 0) {
        id = m_freeEntityIDsPool.back();
        m_freeEntityIDsPool.pop_back();
    }
    else {
        id = m_entities.size();
    }
    auto entity = std::make_unique<Entity>(id);
    Entity* ep = entity.get();
    m_entities[id] = std::move(entity);
    logInfo("Created entity: " + std::to_string(id));

    return ep;
}

void ECSWorld::DestroyEntity(int id)
{
    m_entities.erase(id);
    m_freeEntityIDsPool.push_back(id);
    logInfo("Removed entity: " + std::to_string(id));
}

Entity* ECSWorld::FindEntityByName(const std::string& name)
{
    for (auto& it : m_entities)
    {
        Entity* entity = it.second.get();
        if (entity->GetName() == name)
            return entity;
    }
    return nullptr;
}
