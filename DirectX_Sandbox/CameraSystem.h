#pragma once
#include "System.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "ECSWorld.h"
#include "InputManager.h"


class CameraSystem : public System
{
	void Initialize() override;
	bool IsEntityHasRequiredComponents(const Entity* entity) const override;
	void UpdateEntity(Entity* entity) override;
};