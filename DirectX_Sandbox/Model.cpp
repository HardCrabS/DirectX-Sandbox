#include "Model.h"
#include <sstream>

#include "Graphics.h"
#include "SolidMaterial.h"
#include "SurfaceMaterial.h"

void Model::LoadModel(const std::string filename, vector<XMFLOAT3>& vertices, vector<XMFLOAT4>& colors,
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

std::pair<MeshData, Material*> Model::LoadModel(const std::string& filename)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename,
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace |
		aiProcess_GenSmoothNormals);

	logInfo("[Model] ============= Loading model: " + filename);

	MeshData meshData;
	Material* material = nullptr;

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		logError(std::string("ASSIMP::") + importer.GetErrorString());
		// TODO: make proper exception instead of assert
		assert(false && "Could not open the file");
		return std::make_pair(meshData, material);
	}

	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;

	logInfo("[Model] Number of meshed found: " + std::to_string(scene->mNumMeshes));
	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		logInfo("[Model] Submesh name: " + std::string(scene->mMeshes[i]->mName.C_Str()) + 
		" numOfVertices: " + std::to_string(scene->mMeshes[i]->mNumVertices));
	}
	aiMesh* mesh = scene->mMeshes[0];
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		Vertex vertex;
		// Vertices
		aiVector3D pos = mesh->mVertices[i];
		vertex.pos = XMFLOAT3(pos.x, pos.y, pos.z);

		// Normals
		aiVector3D normal = mesh->mNormals[i];
		vertex.normal = XMFLOAT3(normal.x, normal.y, normal.z);

		// Texture coordinates
		if (mesh->mTextureCoords[0]) {
			aiVector3D texCoord = mesh->mTextureCoords[0][i];
			vertex.UV = XMFLOAT2(texCoord.x, texCoord.y);
		}

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j) {
			indices.push_back(face.mIndices[j]);
		}
	}

	meshData.SetVertices(vertices);
	meshData.SetIndices(indices);

	logInfo("[Model] Number of materials found: " + std::to_string(scene->mNumMaterials));
	for (int i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* aiMaterial = scene->mMaterials[i];
		aiColor4D color;
		bool foundColor = (AI_SUCCESS == aiGetMaterialColor(aiMaterial, AI_MATKEY_COLOR_DIFFUSE, &color));
		logInfo("[Model] Submaterial: " + std::string(aiMaterial->GetName().C_Str()) + 
			" textureCount: " + std::to_string(aiMaterial->GetTextureCount(aiTextureType_DIFFUSE)) + 
		" foundColor: " + std::to_string(foundColor));
	}

	aiMaterial* aiMaterial = scene->mMaterials[mesh->mMaterialIndex];
	if (aiMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		aiString path;

		if (aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
		{
			const aiTexture* texture = scene->GetEmbeddedTexture(path.C_Str());
			auto resourceContainer = &Graphics::getInstance().GetResourcesContainer();
			if (texture)
			{
				logInfo("[Model] Found embedded texture: " + std::string(path.C_Str()));
				auto shaderResource = resourceContainer->GetTexture(texture->mFilename.C_Str(), texture);
				material = Graphics::getInstance().RegisterMaterial(
					std::make_unique<SurfaceMaterial>(shaderResource));
			}
			else
			{
				logInfo("[Model] Found diffuse texture: " + std::string(path.C_Str()));
				auto shaderResource = resourceContainer->GetTexture(path.C_Str());
				material = Graphics::getInstance().RegisterMaterial(
					std::make_unique<SurfaceMaterial>(shaderResource));
			}
		}

	}
	else
	{
		aiColor4D color;
		if (aiGetMaterialColor(aiMaterial, AI_MATKEY_COLOR_DIFFUSE, &color) == AI_SUCCESS)
		{
			material = Graphics::getInstance().RegisterMaterial(
				std::make_unique<SolidMaterial>(XMFLOAT4(color.r, color.g, color.b, color.a)));
		}
		else
		{
			logInfo("[Model] No textures were found! Pink color returned.");
			material = Graphics::getInstance().RegisterMaterial(
				std::make_unique<SolidMaterial>(XMFLOAT4(1, 0.8f, 0.85f, 1)));
		}
	}

	return std::make_pair(meshData, material);
}