#pragma once
#include <vector>
#include <string>
#include <d3d11.h>
#include "Vertex.h"
#include "ModelLoader.h"

enum PrimitiveType;
class Primitive;

class MeshData
{
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;

private:
	void LoadFromFile(const std::string& path);
	void CreatePrimitive(PrimitiveType primitiveType, bool invert=false);
public:
	MeshData(const std::string& path) { LoadFromFile(path); }
	MeshData(PrimitiveType primitiveType, bool invert=false) { CreatePrimitive(primitiveType, invert); }

	std::vector<Vertex>& GetVertices() { return vertices; }
	std::vector<DWORD>& GetIndices() { return indices; }
};


enum PrimitiveType
{
	Cube,
	Sphere,
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