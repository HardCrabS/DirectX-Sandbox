#pragma once
#include "Material.h"
#include "TransformComponent.h"

struct SkyboxBuffer
{
	DirectX::XMMATRIX ViewProjectionMatrix;
	DirectX::XMFLOAT4 cameraPosition;
};

class SkyboxMaterial : public Material
{
	TransformComponent* cameraTransform;

	ID3D11Buffer* cbSkyboxBuffer;
	SkyboxBuffer skyboxBufferData;

	std::string texturePath;
	ID3D11ShaderResourceView* skyboxCubeTexture;
	ID3D11SamplerState* samplerLinear;

	ID3D11DepthStencilState* DSLessEqual;

public:
	SkyboxMaterial(std::string texturePath, TransformComponent* cameraTransform);
	void UpdateResources(DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix,
		DirectX::XMMATRIX projectionMatrix) override;
	void PostDrawCleanUp() override;

protected:
	void CreateInputLayout() override;
	void CreateBuffers() override;
	void CleanUp() override;
};