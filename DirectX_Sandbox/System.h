#pragma once
#include "Entity.h"
#include "Component.h"


class System
{
public:
	virtual void Initialize() {}
	virtual void Tick() {}
	virtual void UpdateEntity(Entity* entity) {}
	virtual bool IsEntityHasRequiredComponents(const Entity* entity) const { return true; };
};