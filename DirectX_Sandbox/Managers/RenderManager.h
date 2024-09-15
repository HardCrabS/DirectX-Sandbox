#pragma once
#include "Singleton.h"
#include "ECS/System.h"
#include "Managers/ECSWorld.h"
#include "Managers/Graphics.h"
#include "ECS/CameraComponent.h"
#include "ECS/MeshComponent.h"
#include "ECS/DirectionalLightComponent.h"


class RenderManager : public Singleton<RenderManager>
{
	friend class Singleton<RenderManager>;

	CameraComponent* cameraToRenderFrom;

public:
	void Initialize();
	void Render();
	void RenderEntity(Entity* entity, std::shared_ptr<Material> material={});
	void RenderMesh(const CustomMesh& mesh, std::shared_ptr<Material> materialToRenderWith);
private:
	RenderManager() : cameraToRenderFrom(nullptr) {}
	void SetupGpuPipeline(const std::shared_ptr<Material>& material) const;
	void UpdateMaterialLights(const std::shared_ptr<Material>& material) const;
};