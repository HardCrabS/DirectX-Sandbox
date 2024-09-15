#include "MeshData.h"
#include <memory>


void MeshData::CreatePrimitive(PrimitiveType primitiveType, bool invert)
{
	std::unique_ptr<Primitive> primitiveMesh;
	switch (primitiveType)
	{
	case PrimitiveType::Cube:
	{
		primitiveMesh = std::make_unique<CubePrimitive>(invert);
		break;
	}
	case PrimitiveType::ScreenQuad:
	{
		primitiveMesh = std::make_unique<ScreenSpaceQuad>();
		break;
	}
	}
	auto primitiveVertices = primitiveMesh->GetVertices();
	auto primitiveIndices = primitiveMesh->GetIndices();

	SetVertices(primitiveVertices);
	SetIndices(primitiveIndices);
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
	if (!inverseTriangles) {
		return {
			0, 2, 3,
			0, 3, 1,

			8, 4, 5,
			8, 5, 9,

			10, 6, 7,
			10, 7, 11,

			12, 13, 14,
			12, 14, 15,

			16, 17, 18,
			16, 18, 19,

			20, 21, 22,
			20, 22, 23
		};
	}
	return {
		3,2,0,
		1,3,0,

		5,4,8,
		9,5,8,

		7,6,10,
		11,7,10,

		14,13,12,
		15,14,12,

		18,17,16,
		19,18,16,

		22,21,20,
		23,22,20
	};
}

std::vector<Vertex> ScreenSpaceQuad::GetVertices()
{
	return {
		Vertex(-1.0f, -1.0f, 0.0f, 0.0f, 1.0f), // Bottom-left
		Vertex(-1.0f,  1.0f, 0.0f, 0.0f, 0.0f), // Top-left
		Vertex(1.0f,  1.0f, 0.0f, 1.0f, 0.0f), // Top-right
		Vertex(-1.0f, -1.0f, 0.0f, 0.0f, 1.0f), // Bottom-left
		Vertex(1.0f,  1.0f, 0.0f, 1.0f, 0.0f), // Top-right
		Vertex(1.0f, -1.0f, 0.0f, 1.0f, 1.0f)  // Bottom-right
	};
}

std::vector<DWORD> ScreenSpaceQuad::GetIndices()
{
	return std::vector<DWORD>();
}

CustomMesh::CustomMesh(const MeshData& meshData)
{
	this->meshData = meshData;

	auto vertices = meshData.GetVertices();
	auto indices = meshData.GetIndices();

	ID3D11Device* device = Graphics::getInstance().GetDevice();

	if (!indices.empty())
	{
		// index buffer
		D3D11_BUFFER_DESC ibd;
		ZeroMemory(&ibd, sizeof(ibd));

		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.ByteWidth = sizeof(DWORD) * meshData.GetNumOfIndices();
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &indices[0];

		HRESULT hr = device->CreateBuffer(&ibd, &iinitData, &pIndexBuffer);
		assert(SUCCEEDED(hr));
	}

	// vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * meshData.GetNumOfVertices();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &vertices[0];

	HRESULT hr = device->CreateBuffer(&bd, &vertexBufferData, &pVertexBuffer);
	assert(SUCCEEDED(hr));
}
