#pragma once
#include "ECS/System.h"
#include "Managers/ECSWorld.h"
#include "Managers/Graphics.h"
#include "CameraComponent.h"
#include "MeshComponent.h"
#include "DirectionalLightComponent.h"


class RenderSystem : public System
{
	CameraComponent* cameraToRenderFrom;
private:
	void UpdateMaterial(const std::shared_ptr<Material>& material, TransformComponent* transform) const;
	void UpdateMaterialLights(const std::shared_ptr<Material>& material) const;
public:
	RenderSystem() : cameraToRenderFrom(nullptr) {}
	void Initialize() override;
	bool IsEntityHasRequiredComponents(const Entity* entity) const override;
	void UpdateEntity(Entity* entity) override;
};
