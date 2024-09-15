#include "RenderManager.h"
#include "Materials/SolidMaterial.h"

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

void RenderManager::RenderEntity(Entity* entity, std::shared_ptr<Material> materialToRenderWith)
{
    TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();
    MeshComponent* meshComponent = entity->GetComponent<MeshComponent>();

    ID3D11DeviceContext* devcon = Graphics::getInstance().GetDeviceContext();
    // TODO: sort models by material. Set Input layout, shaders etc. only once for a set of models.
    std::vector<MeshData> meshData = meshComponent->GetModel().GetMeshData();
    for (int i = 0; i < meshData.size(); i++)
    {
        MeshData* subMeshData = &meshData[i];
        std::shared_ptr<Material> material = materialToRenderWith ? materialToRenderWith 
            : meshComponent->GetModel().GetMaterial(subMeshData->GetMaterialIndex());

        SetupGpuPipeline(material);

        material->UpdateResources(transformComponent->GetWorldMatrix(), cameraToRenderFrom->viewMatrix,
            cameraToRenderFrom->projectionMatrix);

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

void RenderManager::RenderMesh(const CustomMesh& mesh, std::shared_ptr<Material> materialToRenderWith)
{
    ID3D11DeviceContext* devcon = Graphics::getInstance().GetDeviceContext();

    SetupGpuPipeline(materialToRenderWith);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    ID3D11Buffer* vertexBuffer = mesh.pVertexBuffer;
    devcon->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

    devcon->Draw(mesh.meshData.GetNumOfVertices(), 0);
}

void RenderManager::SetupGpuPipeline(const std::shared_ptr<Material>& material) const
{
    auto devcon = Graphics::getInstance().GetDeviceContext();

    devcon->VSSetShader(material->GetVS(), 0, 0);
    devcon->PSSetShader(material->GetPS(), 0, 0);

    devcon->IASetInputLayout(material->GetVertLayout());
    devcon->IASetPrimitiveTopology(material->GetTopology());
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
