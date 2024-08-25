#pragma once
#include "ECS/Component.h"
#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>
#include "Vertex.h"
#include "Managers/Graphics.h"
#include "MeshData.h"
#include "Materials/Material.h"
#include "Model.h"

class MeshComponent : public Component<MeshComponent>
{
	std::vector<ID3D11Buffer*> pVertexBuffers;
	std::vector<ID3D11Buffer*> pIndexBuffers;

	Model model;

public:
	bool isExcludedFromRaycast = false;
public:
	MeshComponent(const Model& model);
	~MeshComponent();

	const Model& GetModel() const { return model; }
	ID3D11Buffer* GetVertexBuffer(int index) const { return pVertexBuffers[index]; }
	ID3D11Buffer* GetIndexBuffer(int index) const { return pIndexBuffers[index]; }
private:
	void ProcessSubMesh(const MeshData& meshData, int index);
};