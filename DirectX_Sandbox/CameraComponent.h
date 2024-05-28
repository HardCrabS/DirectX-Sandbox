#pragma once
#include "Component.h"
#include <DirectXMath.h>
using namespace DirectX;

class CameraComponent : public Component<CameraComponent>
{
public:
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

	XMVECTOR target;
	XMVECTOR up;

	const float ORBIT_SENSITIVITY = 0.01f;
	const float ZOOM_SENSITIVITY = 0.5f;
	float currentDistance;
	const float MIN_DIST = 6.f;
	const float MAX_DIST = 10.f;

	CameraComponent(const XMVECTOR& target, const XMVECTOR& up,
		float fovDegrees, float aspectRatio, float nearZ, float farZ)
	{
		logInfo("Created CameraComponent");
		this->target = target;
		this->up = up;
		currentDistance = (MIN_DIST + MAX_DIST) / 2;
		viewMatrix = XMMatrixIdentity();
		projectionMatrix = XMMatrixPerspectiveFovLH(fovDegrees, aspectRatio, nearZ, farZ);
	}
};