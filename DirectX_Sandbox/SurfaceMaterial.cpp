#include "SurfaceMaterial.h"

SurfaceMaterial::SurfaceMaterial(LPCWSTR texture0Path) 
	: Material(L"surface.fx", "VS", L"surface.fx", "PS"), texture0Path(texture0Path)
{

}

void SurfaceMaterial::UpdateResources(DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
{
	Material::UpdateResources(worldMatrix, viewMatrix, projectionMatrix);

	devcon->PSSetShaderResources(0, 1, &texture0);
	devcon->PSSetSamplers(0, 1, &samplerLinear);
}

void SurfaceMaterial::CreateInputLayout()
{
	layout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = layout.size();

	HRESULT hr = device->CreateInputLayout(&layout[0], numElements, VS_Buffer->GetBufferPointer(),
		VS_Buffer->GetBufferSize(), &vertLayout);
	assert(!FAILED(hr));
}

void SurfaceMaterial::CreateBuffers()
{
	Material::CreateBuffers();

	// texture resources
	DirectX::TexMetadata metadata;
	DirectX::ScratchImage image;
	HRESULT hr = DirectX::LoadFromWICFile(texture0Path, DirectX::WIC_FLAGS_NONE, &metadata, image);
	assert(!FAILED(hr));
	hr = DirectX::CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), metadata, &texture0);
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
}

void SurfaceMaterial::CleanUp()
{
	Material::CleanUp();

	texture0->Release();
	samplerLinear->Release();
}
