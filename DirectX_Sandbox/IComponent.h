#pragma once

class IComponent 
{
public:
	virtual int GetTypeID() const = 0;

	virtual void SetEntityID(int id) = 0;
	virtual int GetEntityID() const = 0;
};