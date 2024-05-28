#include "Graphics.h"


void Graphics::Init(int width, int height, HINSTANCE hInstance, HWND hwnd)
{
    this->Width = width;
    this->Height = height;

    //Describe Buffer
    DXGI_MODE_DESC bufferDesc;

    ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

    bufferDesc.Width = Width;
    bufferDesc.Height = Height;
    bufferDesc.RefreshRate.Numerator = 60;
    bufferDesc.RefreshRate.Denominator = 1;
    bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    //Describe SwapChain
    DXGI_SWAP_CHAIN_DESC swapChainDesc;

    ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

    swapChainDesc.BufferDesc = bufferDesc;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    //Create SwapChain
    HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
        D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &d3d11Device, NULL, &d3d11DevCon);

    //Create BackBuffer
    ID3D11Texture2D* BackBuffer;
    hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);
    assert(!FAILED(hr));

    //Create Render Target
    hr = d3d11Device->CreateRenderTargetView(BackBuffer, NULL, &renderTargetView);
    BackBuffer->Release();

    //Describe Depth/Stencil Buffer
    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = Width;
    depthStencilDesc.Height = Height;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    d3d11Device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
    d3d11Device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

    // set the render target
    d3d11DevCon->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

    //Create the Viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = Width;
    viewport.Height = Height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    //Set the Viewport
    d3d11DevCon->RSSetViewports(1, &viewport);
}

void Graphics::InitPipeline()
{
    //Compile Shaders from shader file
    HRESULT hr = D3DCompileFromFile(L"shaders.fx", 0, 0, "VS", "vs_4_0", 0, 0, &VS_Buffer, 0);
    hr = D3DCompileFromFile(L"shaders.fx", 0, 0, "PS", "ps_4_0", 0, 0, &PS_Buffer, 0);
    assert(!FAILED(hr));

    //Create the Shader Objects
    hr = d3d11Device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
    assert(!FAILED(hr));
    hr = d3d11Device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);
    assert(!FAILED(hr));

    //Set Vertex and Pixel Shaders
    d3d11DevCon->VSSetShader(VS, 0, 0);
    d3d11DevCon->PSSetShader(PS, 0, 0);

    //Create the Input Layout
    layout = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = layout.size();

    hr = d3d11Device->CreateInputLayout(&layout[0], numElements, VS_Buffer->GetBufferPointer(),
        VS_Buffer->GetBufferSize(), &vertLayout);
    assert(!FAILED(hr));

    d3d11DevCon->IASetInputLayout(vertLayout);

    d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Graphics::CleanUp()
{
    SwapChain->Release();
    d3d11Device->Release();
    d3d11DevCon->Release();
    renderTargetView->Release();
    VS->Release();
    PS->Release();
    VS_Buffer->Release();
    PS_Buffer->Release();
    vertLayout->Release();
    depthStencilView->Release();
    depthStencilBuffer->Release();
}

void Graphics::InitScene()
{
    //std::vector<Vertex> v =
    //{
    //    Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f),
    //    Vertex(-0.5f,  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
    //    Vertex(0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f),
    //    Vertex(0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
    //};
    //std::vector<Vertex> v2 =
    //{
    //    Vertex(-1.f, -1.f, 0.6f, 1.0f, 0.0f, 0.0f, 1.0f),
    //    Vertex(-1.f, 0.5f, 0.6f, 1.0f, 0.0f, 0.0f, 1.0f),
    //    Vertex(1.f,  0.0f, 0.9f, 1.0f, 0.0f, 0.0f, 1.0f),
    //    Vertex(1.f, -1.f, 0.9f, 1.f, 0.0f, 0.0f, 1.0f),
    //};

    //std::vector<DWORD> indices = {
    //    0, 1, 2,
    //    0, 2, 3,
    //};

    //cube1 = std::make_unique<Mesh>(d3d11Device, d3d11DevCon, v, indices);
    //cube2 = std::make_unique<Mesh>(d3d11Device, d3d11DevCon, v2, indices);

    //camera.SetPosition(DirectX::XMVectorSet(0.0f, 0.0f, -2.f, 0.0f));
    //camera.SetTarget(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
    //camera.SetUp(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

    //camera.UpdateViewMatrix();
    //camera.SetProjectionMatrix(0.4f * 3.14f, (float)Width / Height, 1.0f, 1000.0f);
}

void Graphics::ClearScreen()
{
    float bgColor[4] = { 0.0f, 0.0f, 1.f, 1.f };
    d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);
    d3d11DevCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Graphics::Present()
{
    SwapChain->Present(0, 0);
}
