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

	ID3D11ShaderResourceView* skyboxCubeTexture;
	ID3D11SamplerState* samplerLinear;

	ID3D11DepthStencilState* DSLessEqual;

public:
	SkyboxMaterial(ID3D11ShaderResourceView* textureSkybox, TransformComponent* cameraTransform);
	void UpdateResources(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix) override;
	void UpdateLights(const std::vector<DirectionalLight>& dirLights) override {};
	void PostDrawCleanUp() override;

protected:
	void CreateBuffers() override;
	void CleanUp() override;
};