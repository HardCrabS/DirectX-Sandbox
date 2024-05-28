#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <assert.h>
#include <memory>
#include <vector>
#include "Vertex.h"

using namespace DirectX;

class Graphics
{
	IDXGISwapChain* SwapChain;
	ID3D11Device* d3d11Device;
	ID3D11DeviceContext* d3d11DevCon;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilBuffer;

	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;
	ID3D10Blob* VS_Buffer;
	ID3D10Blob* PS_Buffer;
	ID3D11InputLayout* vertLayout;

	std::vector<D3D11_INPUT_ELEMENT_DESC> layout;

	int Width, Height;

private:
	Graphics() {}

public:
	static Graphics& getInstance()
	{
		static Graphics instance;
		return instance;
	}
	Graphics(const Graphics&) = delete;
	void operator=(const Graphics&) = delete;

	void Init(int width, int height, HINSTANCE hInstance, HWND hwnd);
	void InitPipeline();
	void CleanUp();
	void InitScene();
	void ClearScreen();
	void Present();

	int GetWidth() const { return Width; }
	int GetHeight() const { return Height; }

	ID3D11Device* GetDevice() { return d3d11Device; }
	ID3D11DeviceContext* GetDeviceContext() { return d3d11DevCon; }
};