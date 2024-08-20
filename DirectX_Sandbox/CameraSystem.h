#pragma once
#include "System.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "ECSWorld.h"
#include "InputManager.h"


class CameraSystem : public System
{
	InputManager* inputManager;

public:
	void Initialize() override;
	bool IsEntityHasRequiredComponents(const Entity* entity) const override;
	void UpdateEntity(Entity* entity) override;

private:
	void OnKeyPressed(const unsigned char key);
	void ControlOrbitCamera(Entity* entity);
	void ControlFreeCamera(Entity* entity);
};