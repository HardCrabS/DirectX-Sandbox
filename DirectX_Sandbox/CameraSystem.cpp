#include "CameraSystem.h"
#include <algorithm>

void CameraSystem::Initialize()
{
}

bool CameraSystem::IsEntityHasRequiredComponents(const Entity* entity) const
{
	return entity->HasComponents(TransformComponent::TypeID(), CameraComponent::TypeID());
}

void CameraSystem::UpdateEntity(Entity* entity)
{
	auto ecsWorld = &ECSWorld::getInstance();
	TransformComponent* transformComponent = ecsWorld->GetComponent<TransformComponent>(entity->GetID());
	CameraComponent* cameraComponent = ecsWorld->GetComponent<CameraComponent>(entity->GetID());

	auto inputManager = &InputManager::getInstance();

	if (inputManager->GetLastMouseWheelDirection() != 0) {
		cameraComponent->currentDistance +=
			-inputManager->GetLastMouseWheelDirection() * cameraComponent->ZOOM_SENSITIVITY;
		cameraComponent->currentDistance = std::clamp(
			cameraComponent->currentDistance,
			cameraComponent->MIN_DIST,
			cameraComponent->MAX_DIST);

		XMVECTOR offset = transformComponent->GetForward() * cameraComponent->currentDistance;
		transformComponent->SetPosition(DirectX::XMVectorAdd(cameraComponent->target, offset));
	}
	if (inputManager->IsLeftDown()) {
		float dx = -inputManager->GetRawX() * cameraComponent->ORBIT_SENSITIVITY;
		float dy = -inputManager->GetRawY() * cameraComponent->ORBIT_SENSITIVITY;
		if (transformComponent->GetPitch() + dy > XM_PIDIV2 || transformComponent->GetPitch() + dy < -XM_PIDIV2)
			dy = 0;
		transformComponent->Rotate(dx, dy, 0);

		XMVECTOR offset = transformComponent->GetForward() * cameraComponent->currentDistance;
		transformComponent->SetPosition(DirectX::XMVectorAdd(cameraComponent->target, offset));
	}

	cameraComponent->viewMatrix = XMMatrixLookAtLH(
		transformComponent->GetPosition(),
		cameraComponent->target,
		cameraComponent->up
	);
}
