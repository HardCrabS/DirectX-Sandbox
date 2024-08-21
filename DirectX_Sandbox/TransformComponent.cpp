#include "TransformComponent.h"

void TransformComponent::UpdateWorldMatrix()
{
	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(position);
	XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(rotation);
	XMMATRIX scalingMatrix = XMMatrixScalingFromVector(scale);

	worldMatrix = scalingMatrix * rotationMatrix * translationMatrix;

	forward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, rotationMatrix);
	right = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, rotationMatrix);
}

TransformComponent::TransformComponent() :
	position(XMVectorZero()),
	rotation(XMQuaternionIdentity()),
	scale(XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f)),
	worldMatrix(XMMatrixIdentity()),
	forward(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f))
{
	logInfo("Created TransformComponent");
}

void TransformComponent::Translate(const XMVECTOR& translation)
{
	position = XMVectorAdd(position, translation);
	UpdateWorldMatrix();
}

void TransformComponent::Rotate(float dYaw, float dPitch, float dRoll)
{
	yaw += dYaw;
	pitch += dPitch;
	roll += dRoll;

	pitch = fmodf(pitch, DirectX::XM_2PI);
	yaw = fmodf(yaw, DirectX::XM_2PI);
	roll = fmodf(roll, DirectX::XM_2PI);

	//forward = XMVectorSet(
	//	cosf(pitch) * cosf(yaw),
	//	sinf(pitch),
	//	cosf(pitch) * sinf(yaw),
	//	0.0f
	//);

	XMVECTOR quat = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
	rotation = quat;  // Set rotation directly to the new quaternion

	//XMVECTOR quat = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
	//rotation = XMQuaternionMultiply(rotation, quat);
	//rotation = XMQuaternionNormalize(rotation);
	UpdateWorldMatrix();
}

void TransformComponent::Scale(const XMVECTOR& scaling)
{
	scale = XMVectorMultiply(scale, scaling);
	UpdateWorldMatrix();
}

void TransformComponent::SetPosition(const XMVECTOR& pos)
{
	position = pos;
	UpdateWorldMatrix();
}
