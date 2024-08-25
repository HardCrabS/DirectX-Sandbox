#pragma once
#include "ECS/IComponent.h"
#include "Managers/Logger.h"

class ComponentTypeIDCounter
{
public:
	static int counter;
};

template<class T>
class Component : public IComponent
{
	static int typeID;
	int entityID;

public:
	Component() {}
	static int TypeID() { return typeID; }
	int GetTypeID() const override { return typeID; }

	void SetEntityID(int id) override { entityID = id; }
	int GetEntityID() const override { return entityID; }
};

template<class T>
int Component<T>::typeID = ComponentTypeIDCounter::counter++;