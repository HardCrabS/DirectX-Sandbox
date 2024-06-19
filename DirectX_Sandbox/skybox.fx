cbuffer skyboxBuffer
{
    float4x4 viewProjectionMatrix;
    float4 cameraPosition;
};

TextureCube SkyMap;
SamplerState ObjSamplerState;

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 TexCoord : TEXCOORD;
};

VS_OUTPUT VS(float3 inPos : POSITION, float2 inTexCoord : TEXCOORD)
{
    VS_OUTPUT output;
   
    float4 pos = mul(float4(inPos, 1), viewProjectionMatrix);
    // set z to w so that depth becomes 1 upon converting to NDC (z/w=1)
    output.Pos = pos.xyww;
    output.TexCoord = inPos;
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    return SkyMap.Sample(ObjSamplerState, input.TexCoord);
}
