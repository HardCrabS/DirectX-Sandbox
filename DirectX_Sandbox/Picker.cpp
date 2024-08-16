#include "Picker.h"
#include "InputManager.h"
#include "ECSWorld.h"
#include "MeshComponent.h"
#include "TransformComponent.h"


void Picker::Initialize()
{
	InputManager::getInstance().OnLeftMouseButtonClick.Subscribe(std::bind(&Picker::Pick, this, std::placeholders::_1, std::placeholders::_2));

	auto cameraEntity = ECSWorld::getInstance().FindEntityWithComponent<CameraComponent>();
	assert(cameraEntity && "No entity with CameraComponent found!");
	camera = ECSWorld::getInstance().GetComponent<CameraComponent>(cameraEntity->GetID());
}

void Picker::Pick(int x, int y)
{
	// Normalize X and Y Coordinates
	float ndcX = (2.0f * x) / width - 1.0f;
	float ndcY = 1.0f - (2.0f * y) / height;

	logInfo("Pick at ndc: " + std::to_string(ndcX) + ", " + std::to_string(ndcY));

	// Go back to world coords by inverting matrices
	XMMATRIX projectionInverse = XMMatrixInverse(nullptr, camera->projectionMatrix);
	XMMATRIX viewInverse = XMMatrixInverse(nullptr, camera->viewMatrix);

	XMVECTOR nearPlanePointInScreenSpace = DirectX::XMVectorSet(ndcX, ndcY, 0.f, 1.f);
	XMVECTOR nearPlanePointInViewSpace = DirectX::XMVector4Transform(nearPlanePointInScreenSpace, projectionInverse);
	XMVECTOR nearPlanePointInWorldSpace = DirectX::XMVector4Transform(nearPlanePointInViewSpace, viewInverse);

	XMVECTOR farPlanePointInScreenSpace = DirectX::XMVectorSet(ndcX, ndcY, 1.f, 1.f);
	XMVECTOR farPlanePointInViewSpace = DirectX::XMVector4Transform(farPlanePointInScreenSpace, projectionInverse);
	XMVECTOR farPlanePointInWorldSpace = DirectX::XMVector4Transform(farPlanePointInViewSpace, viewInverse);

	XMVECTOR direction = XMVector3Normalize(XMVectorSubtract(farPlanePointInWorldSpace, nearPlanePointInWorldSpace));

	logInfo("farPlanePointInWorldSpace: " + prettyXMVector(farPlanePointInWorldSpace));
	logInfo("nearPlanePointInWorldSpace: " + prettyXMVector(nearPlanePointInWorldSpace));

	logInfo("direction: " + prettyXMVector(direction));

	auto meshEntity = ECSWorld::getInstance().FindEntityWithComponent<MeshComponent>();
	auto transform = ECSWorld::getInstance().GetComponent<TransformComponent>(meshEntity->GetID());

	auto cameraEntity = ECSWorld::getInstance().FindEntityWithComponent<CameraComponent>();
	auto cameraTransform = ECSWorld::getInstance().GetComponent<TransformComponent>(cameraEntity->GetID());

	transform->SetPosition(XMVectorAdd(cameraTransform->GetPosition(), XMVectorScale(direction, 30.f)));
}
