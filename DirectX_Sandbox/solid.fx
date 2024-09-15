#include "base.fx"

cbuffer PS_solidBuffer : register(b1)  // b0 is light
{
    float4 solidColor;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
};

VS_OUTPUT VS(float4 inPos : POSITION)
{
    VS_OUTPUT output;
    float4 worldPosition = mul(inPos, worldMatrix);
    float4 viewSpacePosition = mul(worldPosition, viewMatrix);
    output.Pos = mul(viewSpacePosition, projectionMatrix);
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    return solidColor;
}
