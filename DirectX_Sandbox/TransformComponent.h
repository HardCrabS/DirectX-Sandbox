#pragma once
#include "Component.h"
#include <DirectXMath.h>
using namespace DirectX;


class TransformComponent : public Component<TransformComponent>
{
	XMVECTOR position;
	XMVECTOR rotation;
	float pitch = 0, yaw = 0, roll = 0;
	XMVECTOR scale;
	XMMATRIX worldMatrix;
	XMVECTOR forward;

private:
	void UpdateWorldMatrix();
public:
	TransformComponent();

	void Translate(const XMVECTOR& translation);
	void Rotate(float yaw, float pitch, float roll);
	void Scale(const XMVECTOR& scaling);

	void SetPosition(const XMVECTOR& pos);

	XMVECTOR GetPosition() const { return position; }
	XMVECTOR GetRotation() const { return rotation; }
	XMVECTOR GetScale() const { return scale; }
	XMMATRIX GetWorldMatrix() const { return worldMatrix; }
	float GetYaw() const { return yaw; }
	float GetPitch() const { return pitch; }
	float GetRoll() const { return roll; }
	XMVECTOR GetForward() const { return forward; }
};