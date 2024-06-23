#include "Material.h"

Material::Material(LPCWSTR vsFilename, LPCSTR vsName, LPCWSTR psFilename, LPCSTR psName)
	: vsFilename(vsFilename), vsName(vsName), psFilename(psFilename), psName(psName),
	topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
	logInfo("Created material for shader: " + Utils::ConvertLPCWSTRToString(vsFilename));
}

void Material::Initialize()
{
	CreateShaders();
	CreateInputLayout();
	CreateBuffers();
}

void Material::UpdateResources(DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix,
	DirectX::XMMATRIX projectionMatrix)
{
	DirectX::XMMATRIX WVP = worldMatrix * viewMatrix * projectionMatrix;
	constantBuffer.WVP = DirectX::XMMatrixTranspose(WVP);
	devcon->UpdateSubresource(cbBuffer, 0, NULL, &constantBuffer, 0, 0);
	// TODO: probably should move to a higher level
	devcon->VSSetConstantBuffers(0, 1, &cbBuffer);
}

void Material::CreateShaders()
{
	//Compile Shaders from shader file
	HRESULT hr = D3DCompileFromFile(vsFilename, 0, 0, vsName, "vs_4_0", 0, 0, &VS_Buffer, 0);
	hr = D3DCompileFromFile(psFilename, 0, 0, psName, "ps_4_0", 0, 0, &PS_Buffer, 0);
	assert(SUCCEEDED(hr));

	//Create the Shader Objects
	hr = device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	assert(SUCCEEDED(hr));
	hr = device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);
	assert(SUCCEEDED(hr));
}

void Material::CreateBuffers()
{
	// constant buffer
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(VertexConstantBuffer);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;
	HRESULT hr = device->CreateBuffer(&cbbd, NULL, &cbBuffer);
	assert(SUCCEEDED(hr));
}

void Material::CleanUp()
{
	VS->Release();
	PS->Release();
	VS_Buffer->Release();
	PS_Buffer->Release();
	vertLayout->Release();
	cbBuffer->Release();
}