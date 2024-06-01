cbuffer cbPerObject
{
    float4x4 WVP;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float3 Normal : NORMAL;
};

VS_OUTPUT VS(float4 inPos : POSITION, float4 inColor : COLOR, float4 normal : NORMAL)
{
    VS_OUTPUT output;
    
    output.Pos = mul(inPos, WVP);
    float4 vLightDirection = float4(.0f, 3.0f, -1.0f, 1.0f);
    output.Color = saturate((dot(normalize(vLightDirection), normal)));
    output.Normal = normal;  // TODO: mult by World matrix
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    return input.Color;
}
