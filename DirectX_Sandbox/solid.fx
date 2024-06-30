#include "light.fx"

cbuffer VS_cbPerObject : register(b0)
{
    float4x4 WVP;
};

// b0 is light

cbuffer PS_solidBuffer : register(b1)
{
    float4 solidColor;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Normal : NORMAL;
};

VS_OUTPUT VS(float4 inPos : POSITION, float4 inNormal : NORMAL)
{
    VS_OUTPUT output;
    output.Pos = mul(inPos, WVP);
    output.Normal = inNormal; // TODO: mult by World matrix
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    float3 result;
    for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++)
    {
        if (length(directionalLights[i].direction) == 0)
            break;  // no more lights found
    	result += CalculatePhong(directionalLights[i], input.Normal);
    }

    return float4(result, 1) * solidColor;
}
