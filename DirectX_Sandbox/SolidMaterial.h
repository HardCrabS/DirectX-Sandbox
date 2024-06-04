#pragma once
#include "Material.h"

struct SolidBuffer
{
	DirectX::XMFLOAT4 color;
	SolidBuffer(DirectX::XMFLOAT4 c) : color(c) {}
};

class SolidMaterial : public Material
{
	ID3D11Buffer* cbBuffer;
	SolidBuffer solidBuffer;

protected:
	void CreateInputLayout() override;
	void CreateBuffers() override;
	void CleanUp() override;

public:
	SolidMaterial(DirectX::XMFLOAT4 color) : Material(L"solid.fx", "VS", L"solid.fx", "PS"), solidBuffer(color) {}
	void UpdateResources(DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix,
		DirectX::XMMATRIX projectionMatrix) override;
};