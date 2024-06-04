#pragma once
#include <DirectXMath.h>
using namespace DirectX;

struct Vertex
{
    Vertex() : pos(0,0,0), color(1,1,1,1) {}
    Vertex(float x, float y, float z,
        float cr, float cg, float cb, float ca)
        : pos(x, y, z), color(cr, cg, cb, ca) {}

    XMFLOAT3 pos;
    XMFLOAT4 color;
    XMFLOAT3 normal;
    XMFLOAT2 UV;
};