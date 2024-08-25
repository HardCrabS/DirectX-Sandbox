#include "MeshComponent.h"

MeshComponent::MeshComponent(const Model& model) : model(std::move(model))
{
    logInfo("Created MeshComponent");

    auto meshData = model.GetMeshData();
    int numOfSubMeshes = meshData.size();
    pVertexBuffers.resize(numOfSubMeshes);
    pIndexBuffers.resize(numOfSubMeshes);

    for(int i = 0; i < numOfSubMeshes; i++)
    {
        ProcessSubMesh(meshData[i], i);
    }
}

MeshComponent::~MeshComponent()
{
    assert(pVertexBuffers.size() == pIndexBuffers.size());
	for (int i = 0; i < pVertexBuffers.size(); ++i)
	{
        pVertexBuffers[i]->Release();
        pIndexBuffers[i]->Release();
	}
}

void MeshComponent::ProcessSubMesh(const MeshData& meshData, int index)
{
    auto vertices = meshData.GetVertices();
    auto indices = meshData.GetIndices();

    ID3D11Device* device = Graphics::getInstance().GetDevice();

    // index buffer
    D3D11_BUFFER_DESC ibd;
    ZeroMemory(&ibd, sizeof(ibd));

    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.ByteWidth = sizeof(DWORD) * meshData.GetNumOfIndices();
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &indices[0];

    HRESULT hr = device->CreateBuffer(&ibd, &iinitData, &pIndexBuffers[index]);
    assert(SUCCEEDED(hr));

    // vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(Vertex) * meshData.GetNumOfVertices();
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA vertexBufferData;
    ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
    vertexBufferData.pSysMem = &vertices[0];

    hr = device->CreateBuffer(&bd, &vertexBufferData, &pVertexBuffers[index]);
    assert(SUCCEEDED(hr));
}
