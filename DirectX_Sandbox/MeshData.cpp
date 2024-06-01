#include "MeshData.h"
#include <memory>

void MeshData::LoadFromFile(const std::string& path)
{
	vector<XMFLOAT3> vertices;
	vector<XMFLOAT3> normals; 
	vector<XMFLOAT2> textCoords;
	vector<DWORD> indices;

	ModelLoader::LoadModel(path, vertices, normals, textCoords, indices);

	for (int i = 0; i < vertices.size(); i++)
	{
		Vertex vertex;
		vertex.pos = XMFLOAT3(vertices[i].x, vertices[i].y, vertices[i].z);
		vertex.normal = XMFLOAT3(normals[i].x, normals[i].y, normals[i].z);
		this->vertices.push_back(vertex);
	}
	this->indices = indices;

	logInfo("Loaded model: " + path);
}

void MeshData::CreatePrimitive(PrimitiveType primitiveType)
{
	std::unique_ptr<Primitive> primitiveMesh;
	switch (primitiveType)
	{
	case PrimitiveType::Cube:
	{
		primitiveMesh = std::make_unique<CubePrimitive>();
		break;
	}
	}
	vertices = primitiveMesh->GetVertices();
	indices = primitiveMesh->GetIndices();
}

std::vector<Vertex> CubePrimitive::GetVertices()
{
	return {
		Vertex(1, -1, 1, 0.0f, 1.0f, 1.f, 1.f),
		Vertex(-1, -1, 1, 1.0f, 1.0f, 1.f, 1.f),
		Vertex(1, 1, 1, 0.0f, 1.0f, 1.f, 1.f),
		Vertex(-1, 1, 1, 1.0f, 0.0f, 1.f, 1.f),

		Vertex(1, 1, -1, 0.0f, 1.0f, 1.f, 1.f),
		Vertex(-1, 1, -1, 1.0f, 1.0f, 1.f, 1.f),
		Vertex(1, -1, -1, 0.0f, 1.0f, 1.f, 1.f),
		Vertex(-1, -1, -1, 1.0f, 1.0f, 1.f, 1.f),

		Vertex(1, 1, 1, 0.0f, 1.0f, 1.f, 1.f),
		Vertex(-1, 1, 1, 1.0f, 0.0f, 1.f, 1.f),
		Vertex(1, 1, -1, 0.0f, 1.0f, 1.f, 1.f),
		Vertex(-1, 1, -1, 1.0f, 0.0f, 1.f, 1.f),

		Vertex(1, -1, -1, 0.0f, 1.0f, 1.f, 1.f),
		Vertex(1, -1, 1, 1.0f, 0.0f, 1.f, 1.f),
		Vertex(-1, -1, 1, 1.0f, 1.0f, 1.f, 1.f),
		Vertex(-1, -1, -1, 1.0f, 0.0f, 1.f, 1.f),

		Vertex(-1, -1, 1, 0.0f, 1.0f, 1.f, 1.f),
		Vertex(-1, 1, 1, 0.0f, 1.0f, 1.f, 1.f),
		Vertex(-1, 1, -1, 1.0f, 1.0f, 1.f, 1.f),
		Vertex(-1, -1, -1, 1.0f, 0.0f, 1.f, 1.f),

		Vertex(1, -1, -1, 0.0f, 1.0f, 1.f, 1.f),
		Vertex(1, 1, -1, 0.0f, 1.0f, 1.f, 1.f),
		Vertex(1, 1, 1, 1.0f, 1.0f, 1.f, 1.f),
		Vertex(1, -1, 1, 1.0f, 0.0f, 1.f, 1.f),
	};
}

std::vector<DWORD> CubePrimitive::GetIndices()
{
	return {
		0,2,3,
		0,3,1,

		8,4,5,
		8,5,9,

		10,6,7,
		10,7,11,

		12,13,14,
		12,14,15,

		16,17,18,
		16,18,19,

		20,21,22,
		20,22,23
	};
}
