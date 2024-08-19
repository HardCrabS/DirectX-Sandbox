#pragma once
#include <vector>
#include <memory>
#include "DirectXMath.h"
#include "MeshData.h"
#include "ECSWorld.h"
#include "MeshComponent.h"

class BoundingVolume;
class BoundingSphere;

struct HitData
{
	int entityID;
};

class Raycast
{
public:
	static bool Shoot(DirectX::XMFLOAT3 origin, DirectX::XMVECTOR direction, HitData& hitData);
private:
	static std::vector<std::unique_ptr<BoundingVolume>> GetObjectsToCast();
};

class BoundingVolume
{
public:
	int entityID;
public:
	virtual bool IsIntersectWithRaycat(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction) = 0;
};

class BoundingSphere : public BoundingVolume
{
	DirectX::XMFLOAT3 origin;
	float radius;

public:
	BoundingSphere(const std::vector<MeshData>& meshData, const XMMATRIX& worldMatrix);
	BoundingSphere(DirectX::XMVECTOR center, float radius) : radius(radius) { XMStoreFloat3(&origin, center); }
	bool IsIntersectWithRaycat(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction) override;

	float GetRadius() const { return radius; }
private:
	// calculates average position among all model vertices
	DirectX::XMFLOAT3 CalculateOrigin(const std::vector<MeshData>& meshData) const;
	// the radius is the distance from origin to the furthest vertex
	float CalculateRadius(const std::vector<MeshData>& meshData, DirectX::XMFLOAT3 origin, const XMMATRIX& worldMatrix) const;
};