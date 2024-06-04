#include "ModelLoader.h"
#include <sstream>

void ModelLoader::LoadModel(const std::string filename, vector<XMFLOAT3>& vertices, vector<XMFLOAT4>& colors,
	vector<XMFLOAT3>& normals, vector<XMFLOAT2>& textCoords, vector<DWORD>& indices) {
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filename,
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace |
		aiProcess_GenSmoothNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		logError(std::string("ASSIMP::") + importer.GetErrorString());
		// TODO: make proper exception instead of assert
		assert(false && "Could not open the file");
		return;
	}

	aiMesh* mesh = scene->mMeshes[0];

	vertices.reserve(mesh->mNumVertices);
	normals.reserve(mesh->mNumVertices);
	textCoords.reserve(mesh->mNumVertices);
	indices.reserve(mesh->mNumFaces * 3);

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		// Vertices
		aiVector3D pos = mesh->mVertices[i];
		vertices.push_back(XMFLOAT3(pos.x, pos.y, pos.z));

		// Normals
		aiVector3D normal = mesh->mNormals[i];
		normals.push_back(XMFLOAT3(normal.x, normal.y, normal.z));

		// Texture coordinates
		if (mesh->mTextureCoords[0]) {
			aiVector3D texCoord = mesh->mTextureCoords[0][i];
			textCoords.push_back(XMFLOAT2(texCoord.x, texCoord.y));
		}
		else {
			textCoords.push_back(XMFLOAT2(0.0f, 0.0f));
		}

		// Colors
		if (mesh->HasVertexColors(0)) {
			aiColor4D color = mesh->mColors[0][i];
			colors.push_back(XMFLOAT4(color.r, color.g, color.b, color.a));
		}
		else {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			aiColor4D color(1.0f, 1.0f, 1.0f, 1.0f);
			if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
				colors.push_back(XMFLOAT4(color.r, color.g, color.b, color.a));
			}
			else {
				colors.push_back(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j) {
			indices.push_back(face.mIndices[j]);
		}
	}
}