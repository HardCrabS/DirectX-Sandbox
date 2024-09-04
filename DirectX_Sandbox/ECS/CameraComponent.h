#pragma once
#include "ECS/Component.h"
#include <DirectXMath.h>
using namespace DirectX;

enum CameraControlMode
{
	None,
	Orbit,
	Free
};

class CameraComponent : public Component<CameraComponent>
{
public:
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

	XMVECTOR target;
	XMVECTOR up;

	CameraComponent(const XMVECTOR& target, const XMVECTOR& up,
		float fovDegrees, float aspectRatio, float nearZ, float farZ)
	{
		logInfo("Created CameraComponent");
		this->target = target;
		this->up = up;
		viewMatrix = XMMatrixIdentity();
		projectionMatrix = XMMatrixPerspectiveFovLH(fovDegrees, aspectRatio, nearZ, farZ);
	}
};

// flag component for camera which is being used for rendering
class ActiveCameraComponent : public Component<ActiveCameraComponent>
{
public:
	CameraControlMode controlMode = CameraControlMode::Free;
};

class OrbitCameraComponent : public Component<OrbitCameraComponent>
{
public:
	float orbitSensitivity = 0.01f;
	float zoomSensitivity = 0.5f;
	float currentDistance;
	float minDist = 1.f;
	float maxDist = 20.f;

	OrbitCameraComponent()
	{
		currentDistance = (minDist + maxDist) / 2;
	}
};

class FreeCameraComponent : public Component<FreeCameraComponent>
{
public:
	float moveSpeed = 0.03f;
};