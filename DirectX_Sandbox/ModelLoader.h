#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <windows.h>
#include <DirectXMath.h>
#include <vector>
#include <string>
#include "Logger.h"

using namespace DirectX;
using namespace std;

typedef struct
{
	int vIndex1, vIndex2, vIndex3;
	int tIndex1, tIndex2, tIndex3;
	int nIndex1, nIndex2, nIndex3;
}FaceType;


class ModelLoader
{
public:
	static void LoadModel(const std::string filename, vector<XMFLOAT3>& vertices, vector<XMFLOAT3>& normals,
		vector<XMFLOAT2>& textCoords, vector<DWORD>& indices);
};