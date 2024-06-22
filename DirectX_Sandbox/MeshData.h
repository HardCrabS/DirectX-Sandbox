#pragma once
#include <vector>
#include <string>
#include <d3d11.h>
#include "Vertex.h"

enum PrimitiveType
{
	Cube,
	Sphere,
};
class Primitive;

class MeshData
{
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;
	unsigned int numOfVetrices = 0;
	unsigned int numOfIndices = 0;

	unsigned int materialIndex = 0;

private:
	void CreatePrimitive(PrimitiveType primitiveType, bool invert=false);
public:
	MeshData() {}
	MeshData(PrimitiveType primitiveType, bool invert=false) { CreatePrimitive(primitiveType, invert); }

	void SetVertices(std::vector<Vertex>& v) { vertices = std::move(v); numOfVetrices = vertices.size(); }
	void SetIndices(std::vector<DWORD>& i) { indices = std::move(i); numOfIndices = indices.size(); }
	const std::vector<Vertex>& GetVertices() const { return vertices; }
	const std::vector<DWORD>& GetIndices() const { return indices; }
	unsigned int GetNumOfVertices() const { return numOfVetrices; }
	unsigned int GetNumOfIndices() const { return numOfIndices; }
	void SetMaterialIndex(unsigned int index) { materialIndex = index; }
	unsigned int GetMaterialIndex() const { return materialIndex; }
};

class Primitive
{
public:
	virtual std::vector<Vertex> GetVertices() = 0;
	virtual std::vector<DWORD> GetIndices() = 0;
};

class CubePrimitive : public Primitive
{
	bool inverseTriangles = false;
public:
	CubePrimitive(bool invertTriangles=false) : inverseTriangles(inverseTriangles) {}
	std::vector<Vertex> GetVertices() override;
	std::vector<DWORD> GetIndices() override;
};