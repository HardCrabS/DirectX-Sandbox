cbuffer cbPerObject
{
    float4x4 WVP;
};

cbuffer solidBuffer
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
    output.Pos = mul(inPos, WVP);
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    return solidColor;
}
