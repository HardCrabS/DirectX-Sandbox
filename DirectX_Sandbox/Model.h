#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <windows.h>
#include <DirectXMath.h>
#include <vector>
#include <string>
#include "Logger.h"
#include "MeshData.h"
#include "Material.h"
#include <tuple>

using namespace DirectX;
using namespace std;


class Model
{
	std::vector<MeshData> meshData;
	std::vector<Material*> materials;

public:
	Model(const std::string& filename) { LoadModel(filename); }

	const std::vector<MeshData>& GetMeshData() const { return meshData; }
	Material* GetMaterial(int index) const
	{
		if(index < 0 || index >= materials.size())
		{
			logError("[Model] Material index out of range. Index: " + std::to_string(index) + 
				" size: " + std::to_string(materials.size()));
			assert(false && "Incorrect material index");
		}
		return materials[index];
	}
private:
	void LoadModel(const std::string& filename);
	void ProcessNode(aiNode* node, const aiScene* scene);
	std::pair<MeshData, Material*> ProcessMesh(const aiMesh* mesh, const aiScene* scene);
};