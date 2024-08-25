#include "SkyboxMaterial.h"

SkyboxMaterial::SkyboxMaterial(ID3D11ShaderResourceView* textureSkybox, TransformComponent* cameraTransform)
	: Material(L"skybox.fx", "VS", L"skybox.fx", "PS"),
	cameraTransform(cameraTransform), skyboxCubeTexture(textureSkybox)
{

}

void SkyboxMaterial::UpdateResources(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	viewMatrix.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	skyboxBufferData.ViewProjectionMatrix = DirectX::XMMatrixTranspose(viewMatrix * projectionMatrix);
	DirectX::XMStoreFloat4(&skyboxBufferData.cameraPosition, cameraTransform->GetPosition());

	devcon->UpdateSubresource(cbSkyboxBuffer, 0, NULL, &skyboxBufferData, 0, 0);
	devcon->VSSetConstantBuffers(0, 1, &cbSkyboxBuffer);

	devcon->PSSetShaderResources(0, 1, &skyboxCubeTexture);
	devcon->PSSetSamplers(0, 1, &samplerLinear);

	devcon->OMSetDepthStencilState(DSLessEqual, 0);
}

void SkyboxMaterial::PostDrawCleanUp()
{
	devcon->OMSetDepthStencilState(NULL, 0);
}

void SkyboxMaterial::CreateBuffers()
{
	// cb
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(SkyboxBuffer);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;
	HRESULT hr = device->CreateBuffer(&cbbd, NULL, &cbSkyboxBuffer);
	assert(SUCCEEDED(hr));

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = device->CreateSamplerState(&sampDesc, &samplerLinear);
	assert(SUCCEEDED(hr));

	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	hr = device->CreateDepthStencilState(&dssDesc, &DSLessEqual);
	assert(SUCCEEDED(hr));
}

void SkyboxMaterial::CleanUp()
{
	Material::CleanUp();

	skyboxCubeTexture->Release();
}