#pragma once
#include <bitset>
#include <vector>
#include <assert.h>
#include "ECS/Component.h"
#include "Constants.h"

class ECSWorld;

class Entity
{
	using Components = std::vector<std::unique_ptr<IComponent>>;

	int m_id;
	std::string m_name;
	Components m_components;
	std::bitset<MAX_COMPONENTS_PER_ENTITY> m_componentMask;

public:
	Entity(int id) : m_id(id), m_name("Entity" + std::to_string(id)), m_components(MAX_COMPONENTS_PER_ENTITY) {}
	bool IsEmpty() const { return !m_componentMask.any(); }
	int GetID() { return m_id; }
	void SetName(const std::string& name) { m_name = std::move(name); }
	const std::string& GetName() const { return m_name; }

	template<class T, class ...ARGS>
	T* AddComponent(ARGS&&... componentArgs);
	template<class T>
	void RemoveComponent();
	template<class ...ARGS>
	bool HasComponents(ARGS&&... componentIDs) const;
	template<class T>
	T* GetComponent() const;
};

template<class T, class ...ARGS>
T* Entity::AddComponent(ARGS&&... componentArgs)
{
	auto component = std::make_unique<T>(std::forward<ARGS>(componentArgs)...);

	int componentTypeID = T::TypeID();
	assert(componentTypeID < MAX_COMPONENTS_PER_ENTITY);
	m_componentMask |= 1 << componentTypeID;

	component->SetEntityID(m_id);
	T* componentPtr = component.get();
	m_components[componentTypeID] = std::move(component);

	return componentPtr;
}

template<class ...ARGS>
bool Entity::HasComponents(ARGS&&... componentIDs) const
{
	for (int compID : {componentIDs...}) {
		if (!m_componentMask.test(compID))
			return false;
	}
	return true;
}

template<class T>
T* Entity::GetComponent() const
{
	assert(HasComponents(T::TypeID()) && "No such component on entity!");
	return static_cast<T*>(m_components[T::TypeID()].get());
}

template<class T>
void Entity::RemoveComponent()
{
	assert(HasComponents(T::TypeID()) && "No such component on entity!");
	m_componentMask &= ~(1 << T::TypeID());
}