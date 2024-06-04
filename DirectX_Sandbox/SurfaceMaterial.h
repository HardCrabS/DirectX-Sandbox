#pragma once
#include "Material.h"

class SurfaceMaterial : public Material
{
	// textures
	LPCWSTR texture0Path;
	ID3D11ShaderResourceView* texture0;
	ID3D11SamplerState* samplerLinear;

protected:
	void CreateInputLayout() override;
	void CreateBuffers() override;
	void CleanUp() override;

public:
	SurfaceMaterial(LPCWSTR texture0Path);
	void UpdateResources(DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix,
		DirectX::XMMATRIX projectionMatrix) override;
};