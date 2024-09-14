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
private:
	RenderManager() : cameraToRenderFrom(nullptr) {}
	void RenderEntity(Entity* entity);
	void UpdateMaterial(const std::shared_ptr<Material>& material, TransformComponent* transform) const;
	void UpdateMaterialLights(const std::shared_ptr<Material>& material) const;
};