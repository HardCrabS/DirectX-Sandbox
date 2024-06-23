#include "SolidMaterial.h"

void SolidMaterial::UpdateResources(DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, 
	DirectX::XMMATRIX projectionMatrix)
{
	Material::UpdateResources(worldMatrix, viewMatrix, projectionMatrix);

	devcon->UpdateSubresource(cbBuffer, 0, nullptr, &solidBuffer, 0, 0);
	devcon->PSSetConstantBuffers(0, 1, &cbBuffer);
}

void SolidMaterial::CreateInputLayout()
{
	layout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = layout.size();

	HRESULT hr = device->CreateInputLayout(&layout[0], numElements, VS_Buffer->GetBufferPointer(),
		VS_Buffer->GetBufferSize(), &vertLayout);
	assert(SUCCEEDED(hr));
}

void SolidMaterial::CreateBuffers()
{
	Material::CreateBuffers();

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(SolidBuffer);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;
	HRESULT hr = device->CreateBuffer(&cbbd, nullptr, &cbBuffer);
	assert(SUCCEEDED(hr));
}

void SolidMaterial::CleanUp()
{
	Material::CleanUp();

	cbBuffer->Release();
}