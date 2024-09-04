#pragma once
#include "Material.h"
#include "Managers/Graphics.h"

class SurfaceMaterial : public Material
{
	ID3D11ShaderResourceView* diffuseTexture = nullptr;
	ID3D11ShaderResourceView* normalTexture = nullptr;
	ID3D11SamplerState* samplerLinear = nullptr;

public:
	SurfaceMaterial();
	void SetDiffuse(ID3D11ShaderResourceView* texture) { diffuseTexture = texture; }
	void SetNormal(ID3D11ShaderResourceView* texture) { normalTexture = texture; }
	void UpdateResources(DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix,
		DirectX::XMMATRIX projectionMatrix) override;

protected:
	void CreateBuffers() override;
	void CleanUp() override;
};