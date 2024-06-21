#include "RenderSystem.h"

void RenderSystem::Initialize()
{

}

bool RenderSystem::IsEntityHasRequiredComponents(const Entity* entity) const
{
    return entity->HasComponents(TransformComponent::TypeID(), MeshComponent::TypeID());
}

void RenderSystem::UpdateEntity(Entity* entity)
{
    auto ecsWorld = &ECSWorld::getInstance();

    if (cameraToRenderFrom == nullptr)
    {
        auto cameraEntity = ecsWorld->FindEntityWithComponent<CameraComponent>();
        assert(cameraEntity && "No entity with CameraComponent found!");
        cameraToRenderFrom = ecsWorld->GetComponent<CameraComponent>(cameraEntity->GetID());
    }

    TransformComponent* transformComponent = ecsWorld->GetComponent<TransformComponent>(entity->GetID());
    MeshComponent* meshComponent = ecsWorld->GetComponent<MeshComponent>(entity->GetID());

    ID3D11DeviceContext* devcon = Graphics::getInstance().GetDeviceContext();

    std::vector<MeshData> meshData = meshComponent->GetMeshData();
    for (int i = 0; i < meshData.size(); i++)
    {
        MeshData* subMeshData = &meshData[i];
        Material* material = meshComponent->GetMaterial(subMeshData->GetMaterialIndex());
        UpdateMaterial(material, transformComponent);

        devcon->IASetIndexBuffer(meshComponent->GetIndexBuffer(i), DXGI_FORMAT_R32_UINT, 0);

        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        ID3D11Buffer* vertexBuffer = meshComponent->GetVertexBuffer(i);
        devcon->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

        devcon->DrawIndexed(subMeshData->GetNumOfIndices(), 0, 0);

        material->PostDrawCleanUp();
    }
}

void RenderSystem::UpdateMaterial(Material* material, TransformComponent* transform)
{
    auto devcon = Graphics::getInstance().GetDeviceContext();

    devcon->VSSetShader(material->GetVS(), 0, 0);
    devcon->PSSetShader(material->GetPS(), 0, 0);

    devcon->IASetInputLayout(material->GetVertLayout());
    devcon->IASetPrimitiveTopology(material->GetTopology());

    material->UpdateResources(transform->GetWorldMatrix(), cameraToRenderFrom->viewMatrix,
        cameraToRenderFrom->projectionMatrix);
}