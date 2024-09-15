#pragma once
#include "Material.h"
#include "Managers/Graphics.h"

struct EdgeDrawBuffer
{
	float screenWidth;
	float screenHeight;
	int thickness;
	float padding;
};

class EdgeMaterial : public Material
{
	EdgeDrawBuffer edgeCB;
	ID3D11Buffer* edgeCbBuffer;

public:
	EdgeMaterial(float screenWidth, float screenHeight, int thickness) 
		: Material(L"edge_draw.fx", "VS_Main", L"edge_draw.fx", "PS_Main") { edgeCB = { screenWidth, screenHeight, thickness }; }
	void UpdateResources(DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix,
		DirectX::XMMATRIX projectionMatrix) override {}
	void UpdateConstantBuffer();

protected:
	void CreateBuffers() override;
	void CleanUp() override;
};