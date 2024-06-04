cbuffer cbPerObject
{
    float4x4 WVP;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
};

VS_OUTPUT VS(float4 inPos : POSITION, float4 inColor : COLOR, float4 inNormal : NORMAL, float2 inTexCoord : TEXCOORD)
{
    VS_OUTPUT output;
    
    output.Pos = mul(inPos, WVP);
    float4 vLightDirection = float4(.0f, 3.0f, -1.0f, 1.0f);
    output.Color = saturate((dot(normalize(vLightDirection), inNormal))) * inColor;
    output.Normal = inNormal; // TODO: mult by World matrix
    output.TexCoord = inTexCoord;
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    return ObjTexture.Sample(ObjSamplerState, input.TexCoord);
}
