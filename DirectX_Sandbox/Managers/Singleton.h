#pragma once

template <typename T>
class Singleton
{
protected:
	Singleton() {}

public:
	static T& getInstance()
	{
		static T instance;
		return instance;
	}
	Singleton(const Singleton&) = delete;
	void operator=(const Singleton&) = delete;
};