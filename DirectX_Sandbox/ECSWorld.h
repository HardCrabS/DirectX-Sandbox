#pragma once
#include <unordered_map>
#include <vector>
#include <memory>

#include "Entity.h"
#include "Component.h"
#include "IComponent.h"
#include "System.h"
#include "Constants.h"
#include "Logger.h"

// specific systems =============
#include "CameraSystem.h"
#include "RenderSystem.h"
//===============================


class ECSWorld
{
	std::unordered_map<int, std::unique_ptr<Entity>> m_entities;
	using Components = std::vector<std::unique_ptr<IComponent>>;
	std::unordered_map<int, Components> m_entityComponents;

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
	void AddComponent(int entityID, std::unique_ptr<IComponent> component);//TODO: make generics
	void RemoveComponent(int entityID, int componentTypeID);
	template<class T>
	T* GetComponent(int entityID) const;
	template<class T>
	Entity* FindEntityWithComponent() const;
	template<class T>
	std::vector<T*> FindAllComponentsOfType() const;
};

template<class T>
T* ECSWorld::GetComponent(int entityID) const
{
	assert(m_entities.at(entityID)->HasComponents(T::TypeID()) && "No such component on entity!");
	return static_cast<T*>(m_entityComponents.at(entityID)[T::TypeID()].get());
}

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
	for (auto& it : m_entityComponents)
	{
		auto& comps = it.second;
		for (int i = 0; i < comps.size(); i++)
		{
			auto* comp = comps[i].get();
			if (comp != nullptr && comp->GetTypeID() == T::TypeID())
			{
				componentsOfType.push_back(static_cast<T*>(comp));
			}
		}
	}
	return componentsOfType;
}