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
    
    inPos *= 3;
    
    //output.Pos = mul(inPos, WVP);
    //output.Pos = output.Pos.xyww;
    
    //float4 vertexPosition = mul(inPos, worldMatrix);
    //output.TexCoord = vertexPosition - cameraPosition;

    float4 pos = mul(float4(inPos, 1), viewProjectionMatrix);
    output.Pos = pos;
    output.Pos = pos.xyww;
    output.TexCoord = inPos;
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    //return input.Pos.wwww;
    float4 tex = SkyMap.Sample(ObjSamplerState, input.TexCoord);
    tex.a = 1;
    return tex;
}
