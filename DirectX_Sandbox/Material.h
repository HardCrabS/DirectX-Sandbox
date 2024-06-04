#pragma once
#include <d3d11.h>
#include <vector>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include "Logger.h"
#include "Utils.h"

struct VertexConstantBuffer
{
	DirectX::XMMATRIX WVP;
};

class Material
{
protected:
	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;
	ID3D10Blob* VS_Buffer;
	ID3D10Blob* PS_Buffer;
	ID3D11InputLayout* vertLayout;
	std::vector<D3D11_INPUT_ELEMENT_DESC> layout;
	D3D_PRIMITIVE_TOPOLOGY topology;

	// constant buffers
	ID3D11Buffer* cbBuffer;
	VertexConstantBuffer constantBuffer;

	LPCWSTR vsFilename, psFilename;
	LPCSTR vsName, psName;

	ID3D11Device* device;
	ID3D11DeviceContext* devcon;

protected:
	virtual void CreateShaders();
	virtual void CreateInputLayout() = 0;
	virtual void CreateBuffers();
	virtual void CleanUp();
public:
	Material(LPCWSTR vsFilename, LPCSTR vsName, LPCWSTR psFilename, LPCSTR psName);
	virtual void UpdateResources(DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix,
		DirectX::XMMATRIX projectionMatrix);
	virtual void Initialize();
	void SetDeviceAndDevcon(ID3D11Device* device, ID3D11DeviceContext* devcon) { 
		this->device = device; this->devcon = devcon; 
	}

	ID3D11VertexShader* GetVS() const { return VS; }
	ID3D11PixelShader* GetPS() const { return PS; }
	ID3D11InputLayout* GetVertLayout() const { return vertLayout; }
	D3D_PRIMITIVE_TOPOLOGY GetTopology() const { return topology; }
};