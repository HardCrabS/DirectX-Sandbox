#pragma once
#include "System.h"
#include "ECSWorld.h"
#include "Graphics.h"
#include "CameraComponent.h"
#include "MeshComponent.h"


class RenderSystem : public System
{
	CameraComponent* cameraToRenderFrom;
private:
	void UpdateMaterial(TransformComponent* transform, MeshComponent* mesh);
public:
	RenderSystem() : cameraToRenderFrom(nullptr) {}
	void Initialize() override;
	bool IsEntityHasRequiredComponents(const Entity* entity) const override;
	void UpdateEntity(Entity* entity) override;
};
