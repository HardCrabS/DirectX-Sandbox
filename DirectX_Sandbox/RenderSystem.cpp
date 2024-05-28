#include "RenderSystem.h"
#include "MeshComponent.h"

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

    XMMATRIX WVP = transformComponent->GetWorldMatrix() * 
        cameraToRenderFrom->viewMatrix * cameraToRenderFrom->projectionMatrix;

    meshComponent->constantBuffer.WVP = DirectX::XMMatrixTranspose(WVP);
    devcon->UpdateSubresource(meshComponent->cbBuffer, 0, NULL, &meshComponent->constantBuffer, 0, 0);
    devcon->VSSetConstantBuffers(0, 1, &meshComponent->cbBuffer);

    devcon->IASetIndexBuffer(meshComponent->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    devcon->IASetVertexBuffers(0, 1, &meshComponent->pVertexBuffer, &stride, &offset);

    devcon->DrawIndexed(meshComponent->indexCount, 0, 0);
}
