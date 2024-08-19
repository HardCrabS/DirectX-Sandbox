#include "Raycast.h"

bool Raycast::Shoot(DirectX::XMFLOAT3 origin, DirectX::XMVECTOR direction, HitData& hitData)
{
    auto objectsToCast = GetObjectsToCast();
    logInfo("[Raycast] Found objects to cast: " + std::to_string(objectsToCast.size()));
    for (auto& o : objectsToCast)
    {
        if (o->IsIntersectWithRaycat(XMLoadFloat3(&origin), direction)) 
        {
            hitData.entityID = o->entityID;
            return true;
        }
    }

    return false;
}

std::vector<std::unique_ptr<BoundingVolume>> Raycast::GetObjectsToCast()
{
    std::vector<std::unique_ptr<BoundingVolume>> volumes;

    auto meshComponents = ECSWorld::getInstance().FindAllComponentsOfType<MeshComponent>();
    for (auto mesh : meshComponents)
    {
        if (mesh->isExcludedFromRaycast)
            continue;

        auto transform = ECSWorld::getInstance().GetComponent<TransformComponent>(mesh->GetEntityID());
        auto volume = std::make_unique<BoundingSphere>(mesh->GetModel().GetMeshData(), transform->GetWorldMatrix());

        volume->entityID = mesh->GetEntityID();
        volumes.push_back(std::move(volume));
    }

    return volumes;
}

BoundingSphere::BoundingSphere(const std::vector<MeshData>& meshData, const XMMATRIX& worldMatrix)
{
    origin = CalculateOrigin(meshData);
    auto worldOrigin = XMVector3Transform(XMLoadFloat3(&origin), worldMatrix);
    XMStoreFloat3(&origin, worldOrigin);

    radius = CalculateRadius(meshData, origin, worldMatrix);
}

bool BoundingSphere::IsIntersectWithRaycat(DirectX::XMVECTOR rayOrigin, DirectX::XMVECTOR rayDirection)
{
    XMVECTOR originToCenter = XMVectorSubtract(rayOrigin, XMLoadFloat3(&origin));
    float a = 1;
    float b = 2.0f * XMVector3Dot(rayDirection, originToCenter).m128_f32[0];
    float c = XMVector3Dot(originToCenter, originToCenter).m128_f32[0] - radius * radius;

    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0.0f)
    {
        return false;
    }

    float sqrtDiscriminant = sqrtf(discriminant);
    float t1 = (-b - sqrtDiscriminant) / (2.0f * a);
    float t2 = (-b + sqrtDiscriminant) / (2.0f * a);

    if (t1 >= 0.0f || t2 >= 0.0f)
        return true;

    return false; // Intersection is behind the ray origin
}

DirectX::XMFLOAT3 BoundingSphere::CalculateOrigin(const std::vector<MeshData>& meshData) const
{
    XMVECTOR center = XMVectorZero();
    int totalVertexCount = 0;

    for (auto md : meshData)
    {
        totalVertexCount += md.GetNumOfVertices();
        for (const Vertex& vertex : md.GetVertices())
        {
            XMVECTOR vertexPos = XMLoadFloat3(&vertex.pos);
            center = XMVectorAdd(center, vertexPos);
        }
    }

    center = XMVectorScale(center, 1.0f / totalVertexCount);
    XMFLOAT3 result;
    XMStoreFloat3(&result, center);
    return result;
}

float BoundingSphere::CalculateRadius(const std::vector<MeshData>& meshData, DirectX::XMFLOAT3 origin, 
    const XMMATRIX& worldMatrix) const
{
    float radius = 0.0f;

    for (auto md : meshData)
    {
        for (const Vertex& vertex : md.GetVertices())
        {
            XMVECTOR vertexPos = XMLoadFloat3(&vertex.pos);
            XMVECTOR originVector = XMLoadFloat3(&origin);
            XMVECTOR distanceVec = XMVector3Transform(XMVectorSubtract(vertexPos, originVector), worldMatrix);
            float distance = XMVector3Length(distanceVec).m128_f32[0];

            if (distance > radius)
            {
                radius = distance;
            }
        }
    }
    return radius;
}
