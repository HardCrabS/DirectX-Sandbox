#include "CameraSystem.h"
#include <algorithm>

void CameraSystem::Initialize()
{
	inputManager = &InputManager::getInstance();

	inputManager->OnKeyPressedEvent.Subscribe(std::bind(&CameraSystem::OnKeyPressed, this, std::placeholders::_1));
}

bool CameraSystem::IsEntityHasRequiredComponents(const Entity* entity) const
{
	return entity->HasComponents(TransformComponent::TypeID(), CameraComponent::TypeID(), ActiveCameraComponent::TypeID());
}

void CameraSystem::UpdateEntity(Entity* entity)
{
	auto ecsWorld = &ECSWorld::getInstance();
	TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();
	CameraComponent* cameraComponent = entity->GetComponent<CameraComponent>();
	ActiveCameraComponent* activeCameraComponent = entity->GetComponent<ActiveCameraComponent>();

	switch (activeCameraComponent->controlMode)
	{
		case CameraControlMode::Orbit:
		{
			ControlOrbitCamera(entity);
			break;
		}
		case CameraControlMode::Free:
		{
			ControlFreeCamera(entity);
			break;
		}
	}

	cameraComponent->viewMatrix = XMMatrixLookAtLH(
		transformComponent->GetPosition(),
		cameraComponent->target,
		cameraComponent->up
	);
}

void CameraSystem::OnKeyPressed(const unsigned char key)
{
	auto ecsWorld = &ECSWorld::getInstance();
	Entity* activeCamera = ecsWorld->FindEntityWithComponent<ActiveCameraComponent>();
	assert(activeCamera != nullptr);
	ActiveCameraComponent* activeCameraComponent = activeCamera->GetComponent<ActiveCameraComponent>();

	if (inputManager->IsKeyPressed('O'))
	{
		logInfo("[CameraSystem] Set camera mode to Orbit");
		activeCameraComponent->controlMode = CameraControlMode::Orbit;
	}
	else if (inputManager->IsKeyPressed('F'))
	{
		logInfo("[CameraSystem] Set camera mode to Free");
		activeCameraComponent->controlMode = CameraControlMode::Free;
	}
}

void CameraSystem::ControlOrbitCamera(Entity* entity)
{
	auto ecsWorld = &ECSWorld::getInstance();
	TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();
	CameraComponent* cameraComponent = entity->GetComponent<CameraComponent>();
	OrbitComponent* orbitComponent = entity->GetComponent<OrbitComponent>();

	if (inputManager->GetLastMouseWheelDirection() != 0) {
		orbitComponent->currentDistance +=
			-inputManager->GetLastMouseWheelDirection() * orbitComponent->zoomSensitivity;
		orbitComponent->currentDistance = std::clamp(
			orbitComponent->currentDistance,
			orbitComponent->minDist,
			orbitComponent->maxDist);

		XMVECTOR offset = transformComponent->GetForward() * orbitComponent->currentDistance;
		transformComponent->SetPosition(DirectX::XMVectorAdd(cameraComponent->target, offset));
	}
	if (inputManager->IsRightDown()) {
		float dx = -inputManager->GetRawX() * orbitComponent->orbitSensitivity;
		float dy = -inputManager->GetRawY() * orbitComponent->orbitSensitivity;
		if (transformComponent->GetPitch() + dy > XM_PIDIV2 || transformComponent->GetPitch() + dy < -XM_PIDIV2)
			dy = 0;
		transformComponent->Rotate(dx, dy, 0);

		XMVECTOR offset = transformComponent->GetForward() * orbitComponent->currentDistance;
		transformComponent->SetPosition(DirectX::XMVectorAdd(cameraComponent->target, offset));
	}
}

void CameraSystem::ControlFreeCamera(Entity* entity)
{

}
