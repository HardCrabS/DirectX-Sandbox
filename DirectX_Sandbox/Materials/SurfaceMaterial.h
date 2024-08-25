#pragma once
#include "Material.h"
#include "Managers/Graphics.h"

class SurfaceMaterial : public Material
{
	// textures
	ID3D11ShaderResourceView* texture0;
	ID3D11SamplerState* samplerLinear;

public:
	SurfaceMaterial(ID3D11ShaderResourceView* texture0);
	void UpdateResources(DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix,
		DirectX::XMMATRIX projectionMatrix) override;

protected:
	void CreateBuffers() override;
	void CleanUp() override;
};