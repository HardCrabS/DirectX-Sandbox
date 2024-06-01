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

    UpdateMaterial(transformComponent, meshComponent);

    devcon->IASetIndexBuffer(meshComponent->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    devcon->IASetVertexBuffers(0, 1, &meshComponent->pVertexBuffer, &stride, &offset);

    devcon->DrawIndexed(meshComponent->indexCount, 0, 0);
}

void RenderSystem::UpdateMaterial(TransformComponent* transform, MeshComponent* mesh)
{
    auto devcon = Graphics::getInstance().GetDeviceContext();

    devcon->VSSetShader(mesh->material->GetVS(), 0, 0);
    devcon->PSSetShader(mesh->material->GetPS(), 0, 0);

    devcon->IASetInputLayout(mesh->material->GetVertLayout());
    devcon->IASetPrimitiveTopology(mesh->material->GetTopology());

    mesh->material->UpdateResources(transform->GetWorldMatrix(), cameraToRenderFrom->viewMatrix,
        cameraToRenderFrom->projectionMatrix);
}