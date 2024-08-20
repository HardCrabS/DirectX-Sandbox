#include "CameraSystem.h"
#include <algorithm>

void CameraSystem::Initialize()
{
	inputManager = &InputManager::getInstance();

	inputManager->OnKeyPressedEvent.Subscribe(std::bind(&CameraSystem::OnKeyPressed, this, std::placeholders::_1));
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

	switch (cameraComponent->controlMode)
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
	CameraComponent* cameraComponent = ecsWorld->GetComponent<CameraComponent>(activeCamera->GetID());

	if (inputManager->IsKeyPressed('O'))
	{
		logInfo("[CameraSystem] Set camera mode to Orbit");
		cameraComponent->controlMode = CameraControlMode::Orbit;
	}
	else if (inputManager->IsKeyPressed('F'))
	{
		logInfo("[CameraSystem] Set camera mode to Free");
		cameraComponent->controlMode = CameraControlMode::Free;
	}
}

void CameraSystem::ControlOrbitCamera(Entity* entity)
{
	auto ecsWorld = &ECSWorld::getInstance();
	TransformComponent* transformComponent = ecsWorld->GetComponent<TransformComponent>(entity->GetID());
	CameraComponent* cameraComponent = ecsWorld->GetComponent<CameraComponent>(entity->GetID());
	OrbitComponent* orbitComponent = ecsWorld->GetComponent<OrbitComponent>(entity->GetID());

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
