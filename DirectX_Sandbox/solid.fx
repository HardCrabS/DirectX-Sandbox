#include "base.fx"
#include "light.fx"

cbuffer PS_solidBuffer : register(b1)  // b0 is light
{
    float4 solidColor;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 ViewSpacePos : POSITION_VS;
    float3 Normal : NORMAL_VS;
};

VS_OUTPUT VS(float4 inPos : POSITION, float4 inNormal : NORMAL)
{
    VS_OUTPUT output;
    float4 worldPosition = mul(inPos, worldMatrix);
    float4 viewSpacePosition = mul(worldPosition, viewMatrix);
    output.Pos = mul(viewSpacePosition, projectionMatrix);
    output.ViewSpacePos = viewSpacePosition;

    float3 worldNormal = mul(inNormal, (float3x3)worldMatrix);
    output.Normal = normalize(worldNormal);
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    float3 viewDir = normalize(-input.ViewSpacePos);
    float3 lightAffect = CalculateTotalDirectionalLightAffect(input.Normal);

    return float4(lightAffect, 1) * solidColor;
}
