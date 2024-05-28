#include "MeshComponent.h"

MeshComponent::MeshComponent(std::vector<Vertex>& vertices, std::vector<DWORD>& indices)
{
    logInfo("Created MeshComponent");

    this->vertexCount = vertices.size();
    this->indexCount = indices.size();

    ID3D11Device* device = Graphics::getInstance().GetDevice();

    // index buffer
    D3D11_BUFFER_DESC ibd;
    ZeroMemory(&ibd, sizeof(ibd));

    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.ByteWidth = sizeof(DWORD) * indexCount;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &indices[0];

    HRESULT hr = device->CreateBuffer(&ibd, &iinitData, &pIndexBuffer);
    assert(!FAILED(hr));

    // vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(Vertex) * vertexCount;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA vertexBufferData;
    ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
    vertexBufferData.pSysMem = &vertices[0];

    hr = device->CreateBuffer(&bd, &vertexBufferData, &pVertexBuffer);
    assert(!FAILED(hr));

    // constant buffer
    // TODO: move to material
    D3D11_BUFFER_DESC cbbd;
    ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
    cbbd.Usage = D3D11_USAGE_DEFAULT;
    cbbd.ByteWidth = sizeof(VertexConstantBuffer);
    cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbbd.CPUAccessFlags = 0;
    cbbd.MiscFlags = 0;
    hr = device->CreateBuffer(&cbbd, NULL, &cbBuffer);
    assert(!FAILED(hr));
}

MeshComponent::~MeshComponent()
{
    pVertexBuffer->Release();
    pIndexBuffer->Release();
    cbBuffer->Release();
}
