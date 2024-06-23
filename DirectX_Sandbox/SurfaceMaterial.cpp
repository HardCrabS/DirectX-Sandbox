#include "SurfaceMaterial.h"

SurfaceMaterial::SurfaceMaterial(ID3D11ShaderResourceView* texture0)
	: Material(L"surface.fx", "VS", L"surface.fx", "PS"), texture0(texture0)
{
}

void SurfaceMaterial::UpdateResources(DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
{
	Material::UpdateResources(worldMatrix, viewMatrix, projectionMatrix);

	if (texture0 != nullptr)
		devcon->PSSetShaderResources(0, 1, &texture0);
	devcon->PSSetSamplers(0, 1, &samplerLinear);
}

void SurfaceMaterial::CreateBuffers()
{
	Material::CreateBuffers();

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = device->CreateSamplerState(&sampDesc, &samplerLinear);
	assert(SUCCEEDED(hr));
}

void SurfaceMaterial::CleanUp()
{
	Material::CleanUp();

	samplerLinear->Release();
}
