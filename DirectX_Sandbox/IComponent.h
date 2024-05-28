#pragma once

class IComponent 
{
public:
	virtual int GetTypeID() const = 0;
};