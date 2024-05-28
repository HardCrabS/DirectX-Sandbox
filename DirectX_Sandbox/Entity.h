#pragma once
#include <bitset>
#include <assert.h>
#include "Component.h"
#include "Constants.h"


class Entity
{
	int m_id;
	std::bitset<MAX_COMPONENTS_PER_ENTITY> m_componentMask;

public:
	Entity(int id) : m_id(id) {}
	void AddComponent(int componentTypeID);
	void RemoveComponent(int componentTypeID);
	template<class ...ARGS>
	bool HasComponents(ARGS&&... componentIDs) const;
	bool IsEmpty() const { return !m_componentMask.any(); }
	int GetID() { return m_id; }
};

template<class ...ARGS>
bool Entity::HasComponents(ARGS&&... componentIDs) const
{
	for (int compID : {componentIDs...}) {
		if (!m_componentMask.test(compID))
			return false;
	}
	return true;
}