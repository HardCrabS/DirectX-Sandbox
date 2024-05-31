#include "GameWorld.h"
#include <iostream>
#include "MeshComponent.h"
#include "MeshData.h"
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
	//auto meshComponent = std::make_unique<MeshComponent>(MeshData(PrimitiveType::Cube));
	auto meshComponent = std::make_unique<MeshComponent>(MeshData("../Assets/Models/CigaretteBox.obj"));
	auto transformComponent = std::make_unique<TransformComponent>();
	transformComponent->Translate(DirectX::XMVectorSet(.0f, .0f, 0.0f, 0.0f));
	ecsWorld->AddComponent(rectEntity1->GetID(), std::move(meshComponent));
	ecsWorld->AddComponent(rectEntity1->GetID(), std::move(transformComponent));

	//Entity* rectEntity2 = ecsWorld->CreateEntity();
	//auto meshComponent2 = std::make_unique<MeshComponent>(MeshData(PrimitiveType::Cube));
	//auto transformComponent2 = std::make_unique<TransformComponent>();
	//ecsWorld->AddComponent(rectEntity2->GetID(), std::move(meshComponent2));
	//ecsWorld->AddComponent(rectEntity2->GetID(), std::move(transformComponent2));
}

void GameWorld::Update()
{
	ECSWorld::getInstance().Update();
}
