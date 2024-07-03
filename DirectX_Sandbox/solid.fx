#include "light.fx"

cbuffer VS_cbPerObject : register(b0)
{
    float4x4 worldMatrix;
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
};

// b0 is light

cbuffer PS_solidBuffer : register(b1)
{
    float4 solidColor;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 ViewSpacePos : POSITION_VS;
    float3 ViewSpaceNormal : NORMAL_VS;
};

VS_OUTPUT VS(float4 inPos : POSITION, float4 inNormal : NORMAL)
{
    VS_OUTPUT output;
    float4 worldPosition = mul(inPos, worldMatrix);
    float4 viewSpacePosition = mul(worldPosition, viewMatrix);
    output.Pos = mul(viewSpacePosition, projectionMatrix);
    output.ViewSpacePos = viewSpacePosition;

    float3 worldNormal = mul(inNormal, (float3x3)worldMatrix);
    float3 viewSpaceNormal = mul(worldNormal, (float3x3)viewMatrix);
    output.ViewSpaceNormal = normalize(viewSpaceNormal);
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    float3 result;
    float3 viewDir = normalize(-input.ViewSpacePos);
    for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++)
    {
        if (length(directionalLights[i].direction) == 0)
            break;  // no more lights found
        result += CalculatePhong(directionalLights[i], input.ViewSpaceNormal, viewDir);
    }

    return float4(result, 1) * solidColor;
}
