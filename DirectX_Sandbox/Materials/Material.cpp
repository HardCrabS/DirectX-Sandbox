#include "Material.h"
#include <sstream>

Material::Material(LPCWSTR vsFilename, LPCSTR vsName, LPCWSTR psFilename, LPCSTR psName)
	: vsFilename(vsFilename), vsName(vsName), psFilename(psFilename), psName(psName),
	topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST), defaultTextureSampler(nullptr)
{
	logInfo("Created material for shader: " + Utils::ConvertLPCWSTRToString(vsFilename));
}

void Material::Initialize()
{
	CreateShaders();
	CreateInputLayout();
	CreateBuffers();
}

void Material::SetTexture(ID3D11ShaderResourceView* texture)
{
	devcon->PSSetShaderResources(0, 1, &texture);
	auto sampler = GetDefaultTextureSampler();
	devcon->PSSetSamplers(0, 1, &sampler);
}

void Material::UpdateResources(DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix,
	DirectX::XMMATRIX projectionMatrix)
{
	vertexConstantBuffer.worldMatrix = DirectX::XMMatrixTranspose(worldMatrix);
	vertexConstantBuffer.viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
	vertexConstantBuffer.projectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);

	devcon->UpdateSubresource(vertexCbBuffer, 0, nullptr, &vertexConstantBuffer, 0, 0);
	// TODO: probably should move to a higher level
	devcon->VSSetConstantBuffers(0, 1, &vertexCbBuffer);
}

void Material::UpdateLights(const std::vector<DirectionalLight>& dirLights)
{
	if (dirLights.size() > MAX_NUM_OF_DIRECTIONAL_LIGHTS)
	{
		std::ostringstream oss;
		oss << "The max number of directional lights supported is " << MAX_NUM_OF_DIRECTIONAL_LIGHTS << ". Found: " << dirLights.size();
		logError(oss.str());
	}

	for (int i = 0; i < dirLights.size(); i++)
	{
		lightConstantBuffer.dirLights[i] = dirLights[i];
		//lightConstantBuffer.dirLights[i].direction = DirectX::XMVector4Transform(
		//	lightConstantBuffer.dirLights[i].direction, DirectX::XMMatrixTranspose(vertexConstantBuffer.viewMatrix));
	}

	devcon->UpdateSubresource(lightCbBuffer, 0, nullptr, &lightConstantBuffer, 0, 0);
	devcon->PSSetConstantBuffers(0, 1, &lightCbBuffer);
}

void Material::CreateShaders()
{
	//Compile Shaders from shader file
	HRESULT hr = D3DCompileFromFile(vsFilename, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, vsName, "vs_4_0", 0, 0, &VS_Buffer, 0);
	ID3DBlob* errorBlob = nullptr;
	hr &= D3DCompileFromFile(psFilename, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, psName, "ps_4_0", 0, 0, &PS_Buffer, &errorBlob);
	if (FAILED(hr)) {
		logError((char*)errorBlob->GetBufferPointer());
	}
	if (errorBlob) errorBlob->Release();
	assert(SUCCEEDED(hr));

	//Create the Shader Objects
	hr = device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	hr &= device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);
	assert(SUCCEEDED(hr));
}

void Material::CreateBuffers()
{
	D3D11_BUFFER_DESC vertexCbDesc;
	ZeroMemory(&vertexCbDesc, sizeof(D3D11_BUFFER_DESC));
	vertexCbDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexCbDesc.ByteWidth = sizeof(VertexConstantBuffer);
	vertexCbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vertexCbDesc.CPUAccessFlags = 0;
	vertexCbDesc.MiscFlags = 0;
	HRESULT hr = device->CreateBuffer(&vertexCbDesc, NULL, &vertexCbBuffer);
	assert(SUCCEEDED(hr));

	D3D11_BUFFER_DESC lightsBuffer;
	ZeroMemory(&lightsBuffer, sizeof(D3D11_BUFFER_DESC));
	lightsBuffer.Usage = D3D11_USAGE_DEFAULT;
	lightsBuffer.ByteWidth = sizeof(LightConstantBuffer);
	lightsBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightsBuffer.CPUAccessFlags = 0;
	lightsBuffer.MiscFlags = 0;
	hr = device->CreateBuffer(&lightsBuffer, NULL, &lightCbBuffer);
	assert(SUCCEEDED(hr));
}

void Material::CreateInputLayout()
{
	layout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 52, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = layout.size();

	HRESULT hr = device->CreateInputLayout(&layout[0], numElements, VS_Buffer->GetBufferPointer(),
		VS_Buffer->GetBufferSize(), &vertLayout);
	assert(SUCCEEDED(hr));
}

void Material::CleanUp()
{
	VS->Release();
	PS->Release();
	VS_Buffer->Release();
	PS_Buffer->Release();
	vertLayout->Release();
	vertexCbBuffer->Release();
	lightCbBuffer->Release();
	if (defaultTextureSampler)
		defaultTextureSampler->Release();
}

ID3D11SamplerState* Material::GetDefaultTextureSampler()
{
	if (defaultTextureSampler == nullptr)
	{
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		HRESULT hr = device->CreateSamplerState(&sampDesc, &defaultTextureSampler);
		assert(SUCCEEDED(hr));
	}

	return defaultTextureSampler;
}
