#include "Model.h"
#include <sstream>
#include "Graphics.h"
#include "SolidMaterial.h"
#include "SurfaceMaterial.h"


void logNodeHierarchy(aiNode* node);
void LogMaterialInfo(const aiMaterial* material, unsigned int index);


void Model::LoadModel(const std::string& filename)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename,
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace |
		aiProcess_GenSmoothNormals);

	logInfo("[Model] ============= Loading model: " + filename);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		logError(std::string("ASSIMP::") + importer.GetErrorString());
		// TODO: make proper exception instead of assert
		assert(false && "Could not open the file");
		return;
	}

	meshData.resize(scene->mNumMeshes);
	materials.resize(scene->mNumMaterials);

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		MeshData processedSubMesh = ProcessMesh(mesh, scene);
		std::shared_ptr<Material> material = ProcessMaterial(mesh, scene);

		meshData[node->mMeshes[i]] = processedSubMesh;
		materials[processedSubMesh.GetMaterialIndex()] = material;
	}

	for (int i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

MeshData Model::ProcessMesh(const aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;

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

	MeshData subMeshData;
	subMeshData.SetVertices(vertices);
	subMeshData.SetIndices(indices);
	subMeshData.SetMaterialIndex(mesh->mMaterialIndex);
	return subMeshData;
}

std::shared_ptr<Material> Model::ProcessMaterial(const aiMesh* mesh, const aiScene* scene)
{
	auto resourceContainer = &Graphics::getInstance().GetResourcesContainer();
	aiMaterial* aiMaterial = scene->mMaterials[mesh->mMaterialIndex];
	std::ostringstream oss;
	oss << scene->mName.C_Str() << "/" << mesh->mName.C_Str() << "/" << aiMaterial->GetName().C_Str();
	std::string matKey = oss.str();

	if (auto mat = resourceContainer->GetMaterial(matKey)) 
	{
		return mat;
	}

	std::shared_ptr<Material> material;

	if (aiMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		aiString path;

		if (aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
		{
			const aiTexture* texture = scene->GetEmbeddedTexture(path.C_Str());
			if (texture)
			{
				logInfo("[Model] Found embedded texture: " + std::string(path.C_Str()));
				auto shaderResource = resourceContainer->GetTexture(texture->mFilename.C_Str(), texture);
				material = std::make_shared<SurfaceMaterial>(shaderResource);
			}
			else
			{
				logInfo("[Model] Found diffuse texture: " + std::string(path.C_Str()));
				auto shaderResource = resourceContainer->GetTexture(path.C_Str());
				material = std::make_shared<SurfaceMaterial>(shaderResource);
			}
		}
	}
	else
	{
		aiColor4D color;
		if (aiGetMaterialColor(aiMaterial, AI_MATKEY_COLOR_DIFFUSE, &color) == AI_SUCCESS)
		{
			material = std::make_shared<SolidMaterial>(XMFLOAT4(color.r, color.g, color.b, color.a));
		}
		else
		{
			logError("[Model] No textures were found! Pink color returned.");
			LogMaterialInfo(aiMaterial, mesh->mMaterialIndex);
			material = std::make_shared<SolidMaterial>(XMFLOAT4(1, 0.8f, 0.85f, 1));
		}
	}

	resourceContainer->RegisterMaterial(matKey, material);
	return material;
}

void logNodeHierarchy(aiNode* node)
{
	logInfo("ChildNode: " + std::string(node->mName.C_Str()));

	logInfo("numOfMeshes: " + std::to_string(node->mNumMeshes));

	for (int i = 0; i < node->mNumChildren; i++)
	{
		logNodeHierarchy(node->mChildren[i]);
	}
}

void LogMaterialInfo(const aiMaterial* material, unsigned int index)
{
	logInfo("*******************************LogMaterialInfo************************************");
	std::ostringstream oss;
	oss << "Material " << index << ":\n";
	logInfo(oss.str());

	aiString path;
	// Check for diffuse texture
	if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
		oss.str(""); oss << "  Diffuse texture: " << path.C_Str();
		logInfo(oss.str());
	}
	else {
		logInfo("  No diffuse texture.");
	}

	// Check for base color texture
	if (material->GetTexture(aiTextureType_BASE_COLOR, 0, &path) == AI_SUCCESS) {
		oss.str(""); oss << "  Base color texture: " << path.C_Str();
		logInfo(oss.str());
	}
	else {
		logInfo("  No base color texture.");
	}

	// Check for other textures (specular, normal, etc.)
	if (material->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS) {
		oss.str(""); oss << "  Specular texture: " << path.C_Str();
		logInfo(oss.str());
	}
	else {
		logInfo("  No specular texture.");
	}

	if (material->GetTexture(aiTextureType_NORMALS, 0, &path) == AI_SUCCESS) {
		oss.str(""); oss << "  Normal texture: " << path.C_Str();
		logInfo(oss.str());
	}
	else {
		logInfo("  No normal texture.");
	}

	aiColor4D color;
	// Check for colors
	if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &color)) {
		oss.str(""); oss << "  Diffuse color: " << color.r << ", " << color.g << ", " << color.b << ", " << color.a;
		logInfo(oss.str());
	}
	else {
		logInfo("  No diffuse color.");
	}

	if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &color)) {
		oss.str(""); oss << "  Specular color: " << color.r << ", " << color.g << ", " << color.b << ", " << color.a;
		logInfo(oss.str());
	}
	else {
		logInfo("  No specular color.");
	}

	if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &color)) {
		oss.str(""); oss << "  Ambient color: " << color.r << ", " << color.g << ", " << color.b << ", " << color.a;
		logInfo(oss.str());
	}
	else {
		logInfo("  No ambient color.");
	}

	// Check for shininess
	float shininess;
	if (AI_SUCCESS == aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess)) {
		oss.str(""); oss << "  Shininess: " << shininess;
		logInfo(oss.str());
	}
	else {
		logInfo("  No shininess.");
	}

	logInfo("*******************************************************************");
}