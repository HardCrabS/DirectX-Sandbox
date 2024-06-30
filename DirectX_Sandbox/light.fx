struct DirectionalLight
{
    float4 direction;
    float4 color;
    float intensity;
};
#define MAX_DIRECTIONAL_LIGHTS 3

cbuffer PS_lightsBuffer : register(b0)
{
    DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
}


float3 CalculatePhong(DirectionalLight directionalLight, float3 normal)
{
    float ambientStrength = 0.1;

    float3 ambient = ambientStrength * directionalLight.color;
    
    float3 norm = normalize(normal);
    float3 lightDir = directionalLight.direction;

    float diff = max(-dot(norm, lightDir), 0.0);
    float3 diffuse = diff * directionalLight.intensity * directionalLight.color;
    
    return (ambient + diffuse);
}