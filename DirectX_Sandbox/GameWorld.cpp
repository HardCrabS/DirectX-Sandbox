#include "GameWorld.h"
#include "MeshComponent.h"
#include "MeshData.h"
#include "Material.h"
#include "CameraComponent.h"
#include "TransformComponent.h"

void GameWorld::InitScene()
{
	ECSWorld* ecsWorld = &ECSWorld::getInstance();
	ecsWorld->Initialize();

	Entity* cameraEntity = ecsWorld->CreateEntity();
	auto cameraTransformComponent = std::make_unique<TransformComponent>();
	cameraTransformComponent->Translate(DirectX::XMVectorSet(0.0f, 0.0f, -4.f, 0.0f));
	auto cameraComponent = std::make_unique<CameraComponent>(
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),
		0.4f * 3.14f, (float)Graphics::getInstance().GetWidth() / Graphics::getInstance().GetHeight(), 1.0f, 1000.0f);
	ecsWorld->AddComponent(cameraEntity->GetID(), std::move(cameraTransformComponent));
	ecsWorld->AddComponent(cameraEntity->GetID(), std::move(cameraComponent));

	Entity* rectEntity1 = ecsWorld->CreateEntity();
	Material* mat1 = Graphics::getInstance().RegisterMaterial(std::make_unique<Material>(
		L"shaders.fx", "VS", L"shaders.fx", "PS"));
	auto meshComponent = std::make_unique<MeshComponent>(MeshData("../Assets/Models/CigaretteBox.obj"), mat1);
	auto transformComponent = std::make_unique<TransformComponent>();
	transformComponent->Scale(DirectX::XMVectorSet(3.0f, 3.0f, 3.0f, 1.0f));
	ecsWorld->AddComponent(rectEntity1->GetID(), std::move(meshComponent));
	ecsWorld->AddComponent(rectEntity1->GetID(), std::move(transformComponent));

	Entity* rectEntity2 = ecsWorld->CreateEntity();
	Material* mat2 = Graphics::getInstance().RegisterMaterial(std::make_unique<Material>(
		L"solid.fx", "VS", L"solid.fx", "PS"));
	auto meshComponent2 = std::make_unique<MeshComponent>(MeshData(PrimitiveType::Cube), mat2);
	auto transformComponent2 = std::make_unique<TransformComponent>();
	transformComponent2->Translate(DirectX::XMVectorSet(-5.0f, .0f, 0.0f, 0.0f));
	ecsWorld->AddComponent(rectEntity2->GetID(), std::move(meshComponent2));
	ecsWorld->AddComponent(rectEntity2->GetID(), std::move(transformComponent2));
}

void GameWorld::Update()
{
	ECSWorld::getInstance().Update();
}
