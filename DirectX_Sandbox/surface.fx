#include "base.fx"
#include "light.fx"

// b0 is light

cbuffer PS_colorBuffer : register(b1)
{
    float4 gColor;
};

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
    output.Normal = normalize(mul(inNormal, (float3x3) worldMatrix));
    output.Tangent = normalize(mul(inTangent, (float3x3) worldMatrix));
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    float4 finalColor = gColor;

    finalColor *= diffuseTexture.Sample(samplerState, input.TexCoord);
    
    float3 tangentNormal = normalTexture.Sample(samplerState, input.TexCoord).rgb * 2.0f - 1.0f;
    // re-orthogonalize T with respect to N
    input.Tangent = normalize(input.Tangent - dot(input.Tangent, input.Normal) * input.Normal);
    // then retrieve perpendicular vector B with the cross product of T and N
    float3 bitangent = cross(input.Normal, input.Tangent);
    float3x3 TBNMatrix = float3x3(
        input.Tangent,
        bitangent,
        input.Normal);
    
    input.Normal = normalize(mul(tangentNormal, TBNMatrix));
    
    float3 lightAffect = CalculateTotalDirectionalLightAffect(input.Normal);

    return finalColor * float4(lightAffect, 1);
}
