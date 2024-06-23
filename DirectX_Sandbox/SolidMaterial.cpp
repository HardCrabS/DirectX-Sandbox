#include "SolidMaterial.h"

void SolidMaterial::UpdateResources(DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, 
	DirectX::XMMATRIX projectionMatrix)
{
	Material::UpdateResources(worldMatrix, viewMatrix, projectionMatrix);

	devcon->UpdateSubresource(cbBuffer, 0, nullptr, &solidBuffer, 0, 0);
	devcon->PSSetConstantBuffers(0, 1, &cbBuffer);
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