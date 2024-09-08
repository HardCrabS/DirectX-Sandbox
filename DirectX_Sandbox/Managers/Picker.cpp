#include "Picker.h"
#include "InputManager.h"
#include "ECSWorld.h"
#include "ECS/MeshComponent.h"
#include "ECS/TransformComponent.h"


void Picker::Initialize()
{
	auto inputMgr = &InputManager::getInstance();
	inputMgr->OnLeftMouseButtonClick.Subscribe(std::bind(&Picker::Pick, this, std::placeholders::_1, std::placeholders::_2));
	inputMgr->OnRightMouseButtonClick.Subscribe(std::bind(&Picker::OnRMBClicked, this, std::placeholders::_1, std::placeholders::_2));
	inputMgr->OnKeyPressedEvent.Subscribe(std::bind(&Picker::OnKeyPressed, this, std::placeholders::_1));

	auto cameraEntity = ECSWorld::getInstance().FindEntityWithComponent<ActiveCameraComponent>();
	assert(cameraEntity && "No entity with ActiveCameraComponent found!");
	camera = cameraEntity->GetComponent<CameraComponent>();
	cameraTransform = cameraEntity->GetComponent<TransformComponent>();
}

void Picker::Update()
{
	if (!pickedEntity.GetEntity())
		return;
	
	switch (transformMode)
	{
	case TransformMode::Move:
	{
		ProcessMove();
		break;
	}
	case TransformMode::Rotate:
	{
		ProcessRotate();
		break;
	}
	case TransformMode::Scale:
	{
		ProcessScale();
		break;
	}
	}
}

void Picker::Pick(int x, int y)
{
	if (pickedEntity.GetEntity()) {
		// drop whatever is already picked
		transformMode = TransformMode::None;
		transformAxis = TransformAxis::All;
		pickedEntity.Deselect();
		return;
	}

	// Normalize X and Y Coordinates
	float ndcX = (2.0f * x) / width - 1.0f;
	float ndcY = 1.0f - (2.0f * y) / height;

	// Go back to world coords by inverting matrices
	XMMATRIX projectionInverse = XMMatrixInverse(nullptr, camera->projectionMatrix);
	XMMATRIX viewInverse = XMMatrixInverse(nullptr, camera->viewMatrix);

	XMVECTOR nearPlanePointInScreenSpace = DirectX::XMVectorSet(ndcX, ndcY, 0.f, 1.f);
	XMVECTOR nearPlanePointInViewSpace = DirectX::XMVector4Transform(nearPlanePointInScreenSpace, projectionInverse);
	XMVECTOR nearPlanePointInWorldSpace = DirectX::XMVector4Transform(nearPlanePointInViewSpace, viewInverse);
	// Make sure w is 1, therefore ensuring perspective
	nearPlanePointInWorldSpace = XMVectorScale(nearPlanePointInWorldSpace, 1.0f / XMVectorGetW(nearPlanePointInWorldSpace));

	XMVECTOR farPlanePointInScreenSpace = DirectX::XMVectorSet(ndcX, ndcY, 1.f, 1.f);
	XMVECTOR farPlanePointInViewSpace = DirectX::XMVector4Transform(farPlanePointInScreenSpace, projectionInverse);
	XMVECTOR farPlanePointInWorldSpace = DirectX::XMVector4Transform(farPlanePointInViewSpace, viewInverse);
	farPlanePointInWorldSpace = XMVectorScale(farPlanePointInWorldSpace, 1.0f / XMVectorGetW(farPlanePointInWorldSpace));

	XMVECTOR direction = XMVector3Normalize(XMVectorSubtract(farPlanePointInWorldSpace, nearPlanePointInWorldSpace));

	HitData hitData;
	XMFLOAT3 origin;
	XMStoreFloat3(&origin, nearPlanePointInWorldSpace);
	if (Raycast::Shoot(origin, direction, hitData))
	{
		auto hitEntity = ECSWorld::getInstance().GetEntity(hitData.entityID);
		//logInfo("[Picker] Successfuly hit entity: " + hitEntity->GetName());
		pickedEntity.Select(hitEntity);
	}
	else
	{
		return;
	}
}

void Picker::OnKeyPressed(const unsigned char key)
{
	if (!pickedEntity.GetEntity())
		return;

	if (transformMode != TransformMode::None)
	{
		if (key == 'X' || key == 'Y' || key == 'Z')
			pickedEntity.Revert();

		switch (key)
		{
		case 'X':
		{
			transformAxis = TransformAxis::X;
			return;
		}
		case 'Y':
		{
			transformAxis = TransformAxis::Y;
			return;
		}
		case 'Z':
		{
			transformAxis = TransformAxis::Z;
			return;
		}
		}
	}

	auto inputMgr = &InputManager::getInstance();
	mousePosInit = XMVectorSet(inputMgr->GetX(), inputMgr->GetY(), 0, 1);

	switch (key)
	{
	case 'G':
	{
		transformMode = TransformMode::Move;
		break;
	}
	case 'R':
	{
		transformMode = TransformMode::Rotate;
		transformAxis = TransformAxis::X;
		break;
	}
	case 'H':
	{
		transformMode = TransformMode::Scale;
		break;
	}
	}
}

void Picker::OnRMBClicked(int x, int y)
{
	transformMode = TransformMode::None;
	transformAxis = TransformAxis::All;
	pickedEntity.Revert();
}

XMVECTOR Picker::GetAxisVector() const
{
	switch (transformAxis)
	{
	case TransformAxis::X:
	{
		return XMVectorSet(1, 0, 0, 0);
	}
	case TransformAxis::Y:
	{
		return XMVectorSet(0, 1, 0, 0);
	}
	case TransformAxis::Z:
	{
		return XMVectorSet(0, 0, 1, 0);
	}
	}
	return XMVectorSet(1, 1, 1, 0);
}

void Picker::ProcessMove()
{
	auto inputMgr = &InputManager::getInstance();
	float dx = inputMgr->GetRawX();
	float dy = inputMgr->GetRawY();

	XMVECTOR right = cameraTransform->GetRight();
	XMVECTOR up = cameraTransform->GetUp();

	XMVECTOR translation = XMVectorAdd(XMVectorScale(right, dx), XMVectorScale(up, dy));
	translation = XMVectorMultiply(translation, GetAxisVector());

	pickedEntity.GetEntity()->GetComponent<TransformComponent>()->Translate(translation);
}

void Picker::ProcessRotate()
{
	auto inputMgr = &InputManager::getInstance();
	float dx = inputMgr->GetRawX();
	XMVECTOR rotation = XMVectorScale(GetAxisVector(), dx * 0.01f);

	pickedEntity.GetEntity()->GetComponent<TransformComponent>()->Rotate(
		XMVectorGetY(rotation), XMVectorGetX(rotation), XMVectorGetZ(rotation));
}

void Picker::ProcessScale()
{
	auto inputMgr = &InputManager::getInstance();
	XMVECTOR center = XMVectorSet(width / 2.f, height / 2.f, 0, 1);
	XMVECTOR currMousePos = XMVectorSet(inputMgr->GetX(), inputMgr->GetY(), 0, 1);

	XMVECTOR initDistance = XMVector2Length(XMVectorSubtract(mousePosInit, center));
	XMVECTOR currDistance = XMVector2Length(XMVectorSubtract(currMousePos, center));
	XMVECTOR scaleFactor = XMVectorDivide(currDistance, initDistance);
	XMVECTOR resultingScale = XMVectorMultiply(scaleFactor, pickedEntity.GetInitScale());
	auto axisVec = GetAxisVector();
	resultingScale = XMVectorSelect(pickedEntity.GetInitScale(), resultingScale, XMVectorSelectControl(
		XMVectorGetX(axisVec), XMVectorGetY(axisVec), XMVectorGetZ(axisVec), 0)
	);

	pickedEntity.GetEntity()->GetComponent<TransformComponent>()->SetScale(resultingScale);
}
