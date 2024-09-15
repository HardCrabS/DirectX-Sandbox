cbuffer TextureInfoBuffer : register(b0)
{
    float textureWidth;
    float textureHeight;
    int thickness;
};

Texture2D tex;
SamplerState samplerState;

float epsilon = 0.01f;
bool isSameColor(float4 color1, float4 color2)
{
    return (
        (abs(color1.r - color2.r) < epsilon) && 
        (abs(color1.g - color2.g) < epsilon) &&
        (abs(color1.b - color2.b) < epsilon) &&
        (abs(color1.a - color2.a) < epsilon)
    );
}

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

PS_INPUT VS_Main(float4 position : POSITION, float2 texcoord : TEXCOORD)
{
    PS_INPUT output;
    output.position = position;
    output.texcoord = texcoord;
    return output;
}

float4 PS_Main(PS_INPUT input) : SV_Target
{
    float2 texcoord = input.texcoord;
    float4 current = tex.Sample(samplerState, texcoord);
    
    float2 texelSize = 1.0f / float2(textureWidth, textureHeight);

    float4 up = tex.Sample(samplerState, texcoord - float2(texelSize.x, 0) * thickness);
    float4 down = tex.Sample(samplerState, texcoord + float2(texelSize.x, 0) * thickness);
    float4 left = tex.Sample(samplerState, texcoord - float2(0, texelSize.y) * thickness);
    float4 right = tex.Sample(samplerState, texcoord + float2(0, texelSize.y) * thickness);
    
    if (isSameColor(current, up) && isSameColor(current, down) && isSameColor(current, right) && isSameColor(current, left))
    {
        // not on the edge
        discard;
    }
    
    float4 edgeColor = float4(0.99f, 0.73f, 0.01f, 1);
    return edgeColor;
}