#pragma once
#include "Component.h"
#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>
#include "Vertex.h"
#include "Graphics.h"
#include "MeshData.h"

// TODO: move to material
struct VertexConstantBuffer
{
	DirectX::XMMATRIX WVP;
};

class MeshComponent : public Component<MeshComponent>
{
public:
	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;

	// TODO: move to material
	ID3D11Buffer* cbBuffer;
	VertexConstantBuffer constantBuffer;

	int vertexCount;
	int indexCount;

public:
	MeshComponent(MeshData meshData);
	~MeshComponent();
};