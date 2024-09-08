#pragma once
#include "Material.h"
#include "Managers/Graphics.h"

struct ColorBuffer
{
	XMFLOAT4 color;

	ColorBuffer() : color(1, 1, 1, 1) {}
};

class SurfaceMaterial : public Material
{
	ID3D11ShaderResourceView* diffuseTexture = nullptr;
	ID3D11ShaderResourceView* normalTexture = nullptr;
	ID3D11SamplerState* samplerLinear = nullptr;

	ColorBuffer colorCB;
	ID3D11Buffer* colorCbBuffer;

public:
	SurfaceMaterial();
	void SetDiffuse(ID3D11ShaderResourceView* texture) { diffuseTexture = texture; }
	void SetNormal(ID3D11ShaderResourceView* texture) { normalTexture = texture; }
	void SetColor(const XMFLOAT4& c) { colorCB.color = c; }

	void UpdateResources(DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix,
		DirectX::XMMATRIX projectionMatrix) override;

protected:
	void CreateBuffers() override;
	void CleanUp() override;
};