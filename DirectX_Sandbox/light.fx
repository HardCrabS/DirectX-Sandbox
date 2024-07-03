struct DirectionalLight
{
    float4 direction;
    float3 color;
    float intensity;
};
#define MAX_DIRECTIONAL_LIGHTS 3

cbuffer PS_lightsBuffer : register(b0)
{
    DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
}


float3 CalculatePhong(DirectionalLight directionalLight, float3 normal, float3 viewDir)
{
    // all vectors must be in view space
    float ambientStrength = 0.1;

    float3 ambient = ambientStrength;
    
    float3 norm = normalize(normal);
    float3 lightDir = directionalLight.direction;

    float diff = max(-dot(norm, lightDir), 0.0);
    float3 diffuse = diff * directionalLight.intensity;

    float specularStrength = 0.5;
    float reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 4);
    float3 specular = specularStrength * spec;
    
    return (ambient + diffuse + specular) * directionalLight.color;
}