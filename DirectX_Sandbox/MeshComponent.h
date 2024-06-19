#pragma once
#include "Component.h"
#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>
#include "Vertex.h"
#include "Graphics.h"
#include "MeshData.h"
#include "Material.h"
#include "Model.h"

class MeshComponent : public Component<MeshComponent>
{
public:
	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;

	Material* material;

	int vertexCount;
	int indexCount;

public:
	MeshComponent(MeshData meshData, Material* material);
	~MeshComponent();
};