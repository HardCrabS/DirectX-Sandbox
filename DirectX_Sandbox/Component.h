#pragma once
#include "IComponent.h"
#include "Logger.h"

class ComponentTypeIDCounter
{
public:
	static int counter;
};

template<class T>
class Component : public IComponent
{
	static int typeID;

public:
	Component() {}
	static int TypeID() { return typeID; }
	int GetTypeID() const override { return typeID; }
};

template<class T>
int Component<T>::typeID = ComponentTypeIDCounter::counter++;