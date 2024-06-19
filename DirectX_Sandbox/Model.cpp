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

	logInfo("[Model] Loading model: " + filename);

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
		logInfo("[Model] No textures were found! Pink color returned.");
		material = Graphics::getInstance().RegisterMaterial(
			std::make_unique<SolidMaterial>(XMFLOAT4(1, 0.8f, 0.85f, 1)));
	}

	return std::make_pair(meshData, material);
}
//
//void loadModel(string path)
//{
//	Assimp::Importer import;
//	const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
//
//	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
//	{
//		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
//		return;
//	}
//	directory = path.substr(0, path.find_last_of('/'));
//
//	processNode(scene->mRootNode, scene);
//}
//
//void processNode(aiNode* node, const aiScene* scene)
//{
//	// process all the node's meshes (if any)
//	for (unsigned int i = 0; i < node->mNumMeshes; i++)
//	{
//		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//		meshes.push_back(processMesh(mesh, scene));
//	}
//	// then do the same for each of its children
//	for (unsigned int i = 0; i < node->mNumChildren; i++)
//	{
//		processNode(node->mChildren[i], scene);
//	}
//}
//
//Mesh processMesh(aiMesh* mesh, const aiScene* scene)
//{
//	vector<Vertex> vertices;
//	vector<unsigned int> indices;
//	vector<Texture> textures;
//
//	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
//	{
//		Vertex vertex;
//		// process vertex positions, normals and texture coordinates
//		glm::vec3 vector;
//		vector.x = mesh->mVertices[i].x;
//		vector.y = mesh->mVertices[i].y;
//		vector.z = mesh->mVertices[i].z;
//		vertex.Position = vector;
//
//		vector.x = mesh->mNormals[i].x;
//		vector.y = mesh->mNormals[i].y;
//		vector.z = mesh->mNormals[i].z;
//		vertex.Normal = vector;
//
//		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
//		{
//			glm::vec2 vec;
//			vec.x = mesh->mTextureCoords[0][i].x;
//			vec.y = mesh->mTextureCoords[0][i].y;
//			vertex.TexCoords = vec;
//		}
//		else
//			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
//
//			vertices.push_back(vertex);
//	}
//	// process indices
//
//	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
//	{
//		aiFace face = mesh->mFaces[i];
//		for (unsigned int j = 0; j < face.mNumIndices; j++)
//			indices.push_back(face.mIndices[j]);
//	}
//		// process material
//
//	if (mesh->mMaterialIndex >= 0)
//	{
//		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//		vector<Texture> diffuseMaps = loadMaterialTextures(material,
//			aiTextureType_DIFFUSE, "texture_diffuse");
//		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
//		vector<Texture> specularMaps = loadMaterialTextures(material,
//			aiTextureType_SPECULAR, "texture_specular");
//		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
//	}
//
//	return Mesh(vertices, indices, textures);
//}
//
//vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
//{
//	vector<Texture> textures;
//	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
//	{
//		aiString str;
//		mat->GetTexture(type, i, &str);
//		Texture texture;
//		texture.id = TextureFromFile(str.C_Str(), directory);
//		texture.type = typeName;
//		texture.path = str;
//		textures.push_back(texture);
//	}
//	return textures;
//}