#include "SkyboxMaterial.h"

SkyboxMaterial::SkyboxMaterial(LPCWSTR texturePath, TransformComponent* cameraTransform) 
	: Material(L"skybox.fx", "VS", L"skybox.fx", "PS"), texturePath(texturePath), cameraTransform(cameraTransform)
{

}

void SkyboxMaterial::UpdateResources(DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, 
	DirectX::XMMATRIX projectionMatrix)
{
	//Material::UpdateResources(worldMatrix, viewMatrix, projectionMatrix);

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

void SkyboxMaterial::CreateInputLayout()
{
	layout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = layout.size();

	HRESULT hr = device->CreateInputLayout(&layout[0], numElements, VS_Buffer->GetBufferPointer(),
		VS_Buffer->GetBufferSize(), &vertLayout);
	assert(!FAILED(hr));
}

void SkyboxMaterial::CreateBuffers()
{
	//Material::CreateBuffers();

	// cb
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(SkyboxBuffer);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;
	HRESULT hr = device->CreateBuffer(&cbbd, NULL, &cbSkyboxBuffer);
	assert(!FAILED(hr));

	// texture
	DirectX::TexMetadata metadata;
	DirectX::ScratchImage image;
	hr = DirectX::LoadFromDDSFile(texturePath, DirectX::DDS_FLAGS_NONE, &metadata, image);
	assert(!FAILED(hr));
	hr = DirectX::CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), metadata, &skyboxCubeTexture);
	assert(!FAILED(hr));

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
	assert(!FAILED(hr));

	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	hr = device->CreateDepthStencilState(&dssDesc, &DSLessEqual);
	//assert(!FAILED(hr));
}

void SkyboxMaterial::CleanUp()
{
	Material::CleanUp();

	skyboxCubeTexture->Release();
}