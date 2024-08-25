#pragma once
#include "ECS/CameraComponent.h"
#include "Raycast.h"

struct PickedEntity
{
	Entity* entity = nullptr;
	float distanceFromCamera;
};


class Picker
{
	int width, height;
	CameraComponent* camera = nullptr;
	PickedEntity pickedEntity;

public:
	Picker(int screenWidth, int screenHeight) : width(screenWidth), height(screenHeight) {}
	void Initialize();
	void Update();
private:
	void Pick(int x, int y);
};