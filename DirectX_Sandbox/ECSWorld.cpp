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
    m_entityComponents[id] = Components(MAX_COMPONENTS_PER_ENTITY);
    logInfo("Created entity: " + std::to_string(id));

    return ep;
}

void ECSWorld::DestroyEntity(int id)
{
    m_entities.erase(id);
    m_entityComponents.erase(id);
    m_freeEntityIDsPool.push_back(id);
    logInfo("Removed entity: " + std::to_string(id));
}

void ECSWorld::AddComponent(int entityID, std::unique_ptr<IComponent> component)
{
    component->SetEntityID(entityID);
    m_entities[entityID]->AddComponent(component->GetTypeID());
    m_entityComponents[entityID][component->GetTypeID()] = std::move(component);
}

void ECSWorld::RemoveComponent(int entityID, int componentTypeID)
{
    m_entities[entityID]->RemoveComponent(componentTypeID);
}
