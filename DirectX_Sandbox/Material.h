#pragma once
#include <d3d11.h>
#include <vector>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include "Logger.h"
#include "Utils.h"

inline const int MAX_NUM_OF_DIRECTIONAL_LIGHTS = 3;

struct VertexConstantBuffer
{
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projectionMatrix;
};

struct DirectionalLight
{
	DirectX::XMVECTOR direction;
	DirectX::XMFLOAT3 color;
	float intensity;
};

struct LightConstantBuffer
{
	DirectionalLight dirLights[MAX_NUM_OF_DIRECTIONAL_LIGHTS];

	LightConstantBuffer()
	{
		for (auto& dirLight : dirLights) {
			dirLight.direction = DirectX::XMVectorZero();
		}
	}
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
	ID3D11Buffer* vertexCbBuffer;
	VertexConstantBuffer vertexConstantBuffer;
	ID3D11Buffer* lightCbBuffer;
	LightConstantBuffer lightConstantBuffer;

	LPCWSTR vsFilename, psFilename;
	LPCSTR vsName, psName;

	ID3D11Device* device;
	ID3D11DeviceContext* devcon;

public:
	Material(LPCWSTR vsFilename, LPCSTR vsName, LPCWSTR psFilename, LPCSTR psName);
	~Material() { logInfo("Material destroyed: " + Utils::ConvertLPCWSTRToString(vsFilename)); }
	virtual void UpdateResources(DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix,
		DirectX::XMMATRIX projectionMatrix);
	virtual void UpdateLights(const std::vector<DirectionalLight>& dirLights);
	virtual void PostDrawCleanUp() {}
	virtual void Initialize();
	void SetDeviceAndDevcon(ID3D11Device* device, ID3D11DeviceContext* devcon) { 
		this->device = device; this->devcon = devcon; 
	}

	ID3D11VertexShader* GetVS() const { return VS; }
	ID3D11PixelShader* GetPS() const { return PS; }
	ID3D11InputLayout* GetVertLayout() const { return vertLayout; }
	D3D_PRIMITIVE_TOPOLOGY GetTopology() const { return topology; }

protected:
	virtual void CreateShaders();
	virtual void CreateBuffers();
	virtual void CleanUp();
private:
	void CreateInputLayout();
};