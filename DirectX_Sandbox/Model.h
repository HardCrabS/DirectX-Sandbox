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
//public:
//    Model(string path)
//    {
//        loadModel(path);
//    }
//private:
//    // model data
//    vector<Mesh> meshes;
//    string directory;
//
//    void loadModel(string path);
//    void processNode(aiNode* node, const aiScene* scene);
//    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
//    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
public:
	static void LoadModel(const std::string filename, vector<XMFLOAT3>& vertices, vector<XMFLOAT4>& colors, 
		vector<XMFLOAT3>& normals, vector<XMFLOAT2>& textCoords, vector<DWORD>& indices);
	std::pair<MeshData, Material*> LoadModel(const std::string& filename);
};