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
	int numOfVetrices = 0;
	int numOfIndices = 0;

	int materialIndex = 0;

private:
	void LoadFromFile(const std::string& path);
	void CreatePrimitive(PrimitiveType primitiveType, bool invert=false);
public:
	MeshData() {}
	//MeshData(const std::string& path) { LoadFromFile(path); }
	MeshData(PrimitiveType primitiveType, bool invert=false) { CreatePrimitive(primitiveType, invert); }

	void SetVertices(std::vector<Vertex>& v) { vertices = std::move(v); numOfVetrices = vertices.size(); }
	void SetIndices(std::vector<DWORD>& i) { indices = std::move(i); numOfIndices = indices.size(); }
	const std::vector<Vertex>& GetVertices() const { return vertices; }
	const std::vector<DWORD>& GetIndices() const { return indices; }
	int GetNumOfVertices() const { return numOfVetrices; }
	int GetNumOfIndices() const { return numOfIndices; }
	void SetMaterialIndex(int index) { materialIndex = index; }
	int GetMaterialIndex() const { return materialIndex; }
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