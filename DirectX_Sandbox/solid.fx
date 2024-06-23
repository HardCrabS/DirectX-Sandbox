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
    float4 Color : COLOR;
};

VS_OUTPUT VS(float4 inPos : POSITION, float4 inNormal : NORMAL)
{
    VS_OUTPUT output;
    output.Pos = mul(inPos, WVP);
    float4 vLightDirection = float4(-1.0f, 3.0f, 0.0f, 1.0f);
    output.Color = saturate((dot(normalize(vLightDirection), inNormal)));
    //output.Normal = inNormal; // TODO: mult by World matrix
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    return input.Color * solidColor;
}
