#include "RenderManager.h"

void RenderManager::Initialize()
{
    auto ecsWorld = &ECSWorld::getInstance();
    auto cameraEntity = ecsWorld->FindEntityWithComponent<ActiveCameraComponent>();
    assert(cameraEntity && "No entity with ActiveCameraComponent found!");
    cameraToRenderFrom = cameraEntity->GetComponent<CameraComponent>();
}

void RenderManager::Render()
{
    const auto& entities = ECSWorld::getInstance().GetEntities();

    for (const auto& it : entities) {
        auto& e = it.second;
        if (!e->HasComponents(MeshComponent::TypeID()))
            continue;

        RenderEntity(e.get());
    }
}

void RenderManager::RenderEntity(Entity* entity)
{
    TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();
    MeshComponent* meshComponent = entity->GetComponent<MeshComponent>();

    ID3D11DeviceContext* devcon = Graphics::getInstance().GetDeviceContext();

    std::vector<MeshData> meshData = meshComponent->GetModel().GetMeshData();
    for (int i = 0; i < meshData.size(); i++)
    {
        MeshData* subMeshData = &meshData[i];
        std::shared_ptr<Material> material = meshComponent->GetModel().GetMaterial(subMeshData->GetMaterialIndex());
        UpdateMaterial(material, transformComponent);
        UpdateMaterialLights(material);

        devcon->IASetIndexBuffer(meshComponent->GetIndexBuffer(i), DXGI_FORMAT_R32_UINT, 0);

        // TODO: store vertices in a single buffer and make proper indexation
        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        ID3D11Buffer* vertexBuffer = meshComponent->GetVertexBuffer(i);
        devcon->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

        devcon->DrawIndexed(subMeshData->GetNumOfIndices(), 0, 0);

        material->PostDrawCleanUp();
    }
}

void RenderManager::UpdateMaterial(const std::shared_ptr<Material>& material, TransformComponent* transform) const
{
    auto devcon = Graphics::getInstance().GetDeviceContext();

    devcon->VSSetShader(material->GetVS(), 0, 0);
    devcon->PSSetShader(material->GetPS(), 0, 0);

    devcon->IASetInputLayout(material->GetVertLayout());
    devcon->IASetPrimitiveTopology(material->GetTopology());

    material->UpdateResources(transform->GetWorldMatrix(), cameraToRenderFrom->viewMatrix,
        cameraToRenderFrom->projectionMatrix);
}

void RenderManager::UpdateMaterialLights(const std::shared_ptr<Material>& material) const
{
    auto ecsWorld = &ECSWorld::getInstance();
    auto directLights = ecsWorld->FindAllComponentsOfType<DirectionalLightComponent>();

    std::vector<DirectionalLight> directLightsBufferStructs;
    for (auto* dirLight : directLights)
    {
        DirectionalLight directional_light = dirLight->ConvertToConstantBufferStruct();
        directLightsBufferStructs.push_back(directional_light);
    }

    material->UpdateLights(directLightsBufferStructs);
}
