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


float3 CalculateLight(DirectionalLight directionalLight, float3 normal)
{
    float AMBIENT_STRENGTH = 0.3f;

    float3 ambient = AMBIENT_STRENGTH;
    
    float3 norm = normalize(normal);
    float3 lightDir = directionalLight.direction;

    float diff = max(-dot(norm, lightDir), 0.0);
    float3 diffuse = diff * directionalLight.intensity;

    //float specularStrength = 0.01;
    //float reflectDir = reflect(-lightDir, norm);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 4);
    //float3 specular = specularStrength * spec;
    
    return (ambient + diffuse) * directionalLight.color;
}

float3 CalculateTotalDirectionalLightAffect(float3 normal)
{
    float3 result;
    for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++)
    {
        if (length(directionalLights[i].direction) == 0)
            break; // no more lights found
        result += CalculateLight(directionalLights[i], normal);
    }
    return result;
}