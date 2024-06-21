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
	std::vector<ID3D11Buffer*> pVertexBuffers;
	std::vector<ID3D11Buffer*> pIndexBuffers;

	std::vector<MeshData> meshData;
	std::vector<Material*> materials;

public:
	MeshComponent(const std::vector<MeshData>& meshData, const std::vector<Material*>& materials);
	~MeshComponent();

	const std::vector<MeshData>& GetMeshData() const { return meshData; }
	Material* GetMaterial(int index) const { return materials[index]; }
	ID3D11Buffer* GetVertexBuffer(int index) const { return pVertexBuffers[index]; }
	ID3D11Buffer* GetIndexBuffer(int index) const { return pIndexBuffers[index]; }
private:
	void ProcessSubMesh(const MeshData& meshData, int index);
};