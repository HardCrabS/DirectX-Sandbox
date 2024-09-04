#include "base.fx"
#include "light.fx"

Texture2D diffuseTexture : register(t0);
Texture2D normalTexture : register(t1);
SamplerState samplerState;

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL_VS;
    float3 Tangent : TANGENT_VS;
};

VS_OUTPUT VS(float4 inPos : POSITION, float3 inNormal : NORMAL, float3 inTangent : TANGENT, float2 inTexCoord : TEXCOORD)
{
    VS_OUTPUT output;
    float4 worldPosition = mul(inPos, worldMatrix);
    float4 viewSpacePosition = mul(worldPosition, viewMatrix);
    output.Pos = mul(viewSpacePosition, projectionMatrix);
    output.TexCoord = inTexCoord;
    output.Normal = mul(inNormal, (float3x3) worldMatrix);
    output.Tangent = mul(inTangent, (float3x3) worldMatrix);
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    float3 bitangent = normalize(cross(input.Normal, input.Tangent));
    float3x3 TBNMatrix = float3x3(
        input.Tangent,
        bitangent,
        input.Normal
    );
    float3 tangentNormal = normalTexture.Sample(samplerState, input.TexCoord).rgb * 2.0f - 1.0f;
    float3 worldNormal = mul(tangentNormal, TBNMatrix);
    
    float3 lightAffect = CalculateTotalDirectionalLightAffect(worldNormal);

    return float4(lightAffect, 1) * normalTexture.Sample(samplerState, input.TexCoord);
    return float4(lightAffect, 1) * diffuseTexture.Sample(samplerState, input.TexCoord);
}
