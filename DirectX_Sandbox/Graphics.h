#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <assert.h>
#include <memory>
#include <vector>
#include "ResourcesContainer.h"
#include "Vertex.h"
#include "Material.h"

using namespace DirectX;

class Graphics
{
	IDXGISwapChain* SwapChain;
	ID3D11Device* d3d11Device;
	ID3D11DeviceContext* d3d11DevCon;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilBuffer;

	ResourcesContainer resourcesContainer;

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
	void CleanUp();
	void ClearScreen();
	void Present();

	int GetWidth() const { return Width; }
	int GetHeight() const { return Height; }

	ID3D11Device* GetDevice() { return d3d11Device; }
	ID3D11DeviceContext* GetDeviceContext() { return d3d11DevCon; }
	ResourcesContainer& GetResourcesContainer() { return resourcesContainer; }
};