#include "GameWorld.h"
#include <iostream>
#include "MeshComponent.h"
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
	std::vector<Vertex> v =
	{
		Vertex(1, -1, 1, 0.0f, 1.0f, 1.f, 1.f),
		Vertex(-1, -1, 1, 1.0f, 1.0f, 1.f, 1.f),
		Vertex(1, 1, 1, 0.0f, 1.0f, 1.f, 1.f),
		Vertex(-1, 1, 1, 1.0f, 0.0f, 1.f, 1.f),

		Vertex(1, 1, -1, 0.0f, 1.0f, 1.f, 1.f),
		Vertex(-1, 1, -1, 1.0f, 1.0f, 1.f, 1.f),
		Vertex(1, -1, -1, 0.0f, 1.0f, 1.f, 1.f),
		Vertex(-1, -1, -1, 1.0f, 1.0f, 1.f, 1.f),

		Vertex(1, 1, 1, 0.0f, 1.0f, 1.f, 1.f),
		Vertex(-1, 1, 1, 1.0f, 0.0f, 1.f, 1.f),
		Vertex(1, 1, -1, 0.0f, 1.0f, 1.f, 1.f),
		Vertex(-1, 1, -1, 1.0f, 0.0f, 1.f, 1.f),

		Vertex(1, -1, -1, 0.0f, 1.0f, 1.f, 1.f),
		Vertex(1, -1, 1, 1.0f, 0.0f, 1.f, 1.f),
		Vertex(-1, -1, 1, 1.0f, 1.0f, 1.f, 1.f),
		Vertex(-1, -1, -1, 1.0f, 0.0f, 1.f, 1.f),

		Vertex(-1, -1, 1, 0.0f, 1.0f, 1.f, 1.f),
		Vertex(-1, 1, 1, 0.0f, 1.0f, 1.f, 1.f),
		Vertex(-1, 1, -1, 1.0f, 1.0f, 1.f, 1.f),
		Vertex(-1, -1, -1, 1.0f, 0.0f, 1.f, 1.f),

		Vertex(1, -1, -1, 0.0f, 1.0f, 1.f, 1.f),
		Vertex(1, 1, -1, 0.0f, 1.0f, 1.f, 1.f),
		Vertex(1, 1, 1, 1.0f, 1.0f, 1.f, 1.f),
		Vertex(1, -1, 1, 1.0f, 0.0f, 1.f, 1.f),
	};
	std::vector<DWORD> indices = {
		0,2,3,
		0,3,1,

		8,4,5,
		8,5,9,

		10,6,7,
		10,7,11,

		12,13,14,
		12,14,15,

		16,17,18,
		16,18,19,

		20,21,22,
		20,22,23
	};
	auto meshComponent = std::make_unique<MeshComponent>(v, indices);
	auto transformComponent = std::make_unique<TransformComponent>();
	transformComponent->Translate(DirectX::XMVectorSet(-2.0f, 2.0f, 0.0f, 0.0f));
	ecsWorld->AddComponent(rectEntity1->GetID(), std::move(meshComponent));
	ecsWorld->AddComponent(rectEntity1->GetID(), std::move(transformComponent));

	Entity* rectEntity2 = ecsWorld->CreateEntity();
	auto meshComponent2 = std::make_unique<MeshComponent>(v, indices);
	auto transformComponent2 = std::make_unique<TransformComponent>();
	ecsWorld->AddComponent(rectEntity2->GetID(), std::move(meshComponent2));
	ecsWorld->AddComponent(rectEntity2->GetID(), std::move(transformComponent2));
}

void GameWorld::Update()
{
	ECSWorld::getInstance().Update();
}
