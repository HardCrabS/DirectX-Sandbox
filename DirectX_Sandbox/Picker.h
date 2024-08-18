#pragma once
#include "CameraComponent.h"
#include "Raycast.h"


class Picker
{
	int width, height;
	CameraComponent* camera;

public:
	Picker(int screenWidth, int screenHeight) : width(screenWidth), height(screenHeight) {}
	void Initialize();
private:
	void Pick(int x, int y);
};