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
	TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();
	CameraComponent* cameraComponent = entity->GetComponent<CameraComponent>();
	OrbitCameraComponent* orbitComponent = entity->GetComponent<OrbitCameraComponent>();

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
		float dx = inputManager->GetRawX() * orbitComponent->orbitSensitivity;
		float dy = inputManager->GetRawY() * orbitComponent->orbitSensitivity;
		if (transformComponent->GetPitch() + dy > XM_PIDIV2 || transformComponent->GetPitch() + dy < -XM_PIDIV2)
			dy = 0;
		transformComponent->Rotate(dx, dy, 0);

		XMVECTOR offset = transformComponent->GetForward() * orbitComponent->currentDistance;
		transformComponent->SetPosition(DirectX::XMVectorAdd(cameraComponent->target, offset));
	}
}

void CameraSystem::ControlFreeCamera(Entity* entity)
{
	TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();
	CameraComponent* camera = entity->GetComponent<CameraComponent>();
	FreeCameraComponent* freeCamera = entity->GetComponent<FreeCameraComponent>();

	int straightDirection = 0;
	int sideDirection = 0;

	if (inputManager->IsKeyPressed('W')) {
		straightDirection = 1;
	}
	else if (inputManager->IsKeyPressed('S')) {
		straightDirection = -1;
	}
	if (inputManager->IsKeyPressed('A')) {
		sideDirection = -1;
	}
	else if (inputManager->IsKeyPressed('D')) {
		sideDirection = 1;
	}

	if (inputManager->IsRightDown()) {
		float dx = inputManager->GetRawX() * 0.01f;
		float dy = -inputManager->GetRawY() * 0.01f;
		if (transformComponent->GetPitch() + dy > XM_PIDIV2 || transformComponent->GetPitch() + dy < -XM_PIDIV2)
			dy = 0;
		transformComponent->Rotate(dx, dy, 0);
	}

	float forwardSpeed = straightDirection * freeCamera->moveSpeed;
	float sideSpeed = sideDirection * freeCamera->moveSpeed;
	XMVECTOR translation = XMVectorAdd(
		XMVectorScale(transformComponent->GetForward(), forwardSpeed),
		XMVectorScale(transformComponent->GetRight(), sideSpeed)
	);
	transformComponent->Translate(translation);

	camera->target = XMVectorAdd(transformComponent->GetPosition(), transformComponent->GetForward());
}
