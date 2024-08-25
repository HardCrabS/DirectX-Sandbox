#pragma once
#include <unordered_map>
#include <vector>
#include <memory>

#include "ECS/Entity.h"
#include "ECS/Component.h"
#include "ECS/IComponent.h"
#include "ECS/System.h"
#include "Constants.h"
#include "Logger.h"

// specific systems =============
#include "ECS/CameraSystem.h"
#include "ECS/RenderSystem.h"
//===============================


class ECSWorld
{
	std::unordered_map<int, std::unique_ptr<Entity>> m_entities;
	std::vector<int> m_freeEntityIDsPool;
	std::vector<std::unique_ptr<System>> m_systems;

private:
	ECSWorld() {}

public:
	static ECSWorld& getInstance()
	{
		static ECSWorld instance;
		return instance;
	}
	ECSWorld(const ECSWorld&) = delete;
	void operator=(const ECSWorld&) = delete;

	void Initialize();
	void Update();
	Entity* CreateEntity();
	Entity* GetEntity(int id) { return m_entities[id].get(); }
	void DestroyEntity(int id);
	template<class T>
	Entity* FindEntityWithComponent() const;
	template<class T>
	std::vector<T*> FindAllComponentsOfType() const;
	Entity* FindEntityByName(const std::string& name);
};

template<class T>
Entity* ECSWorld::FindEntityWithComponent() const
{
	for (auto& it : m_entities)
	{
		Entity* entity = it.second.get();
		if (entity->HasComponents(T::TypeID())) {
			return entity;
		}
	}
	return nullptr;
}

template<class T>
std::vector<T*> ECSWorld::FindAllComponentsOfType() const
{
	std::vector<T*> componentsOfType;
	for (auto& it : m_entities)
	{
		Entity* entity = it.second.get();
		if (entity->HasComponents(T::TypeID()))
		{
			componentsOfType.push_back(static_cast<T*>(entity->GetComponent<T>()));
		}
	}
	return componentsOfType;
}