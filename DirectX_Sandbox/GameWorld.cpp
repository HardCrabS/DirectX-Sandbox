#include "GameWorld.h"
#include "MeshComponent.h"
#include "MeshData.h"
#include "SurfaceMaterial.h"
#include "SolidMaterial.h"
#include "SkyboxMaterial.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "Model.h"

void GameWorld::InitScene()
{
	ECSWorld* ecsWorld = &ECSWorld::getInstance();
	ecsWorld->Initialize();
	Graphics* graphics = &Graphics::getInstance();

	Entity* cameraEntity = ecsWorld->CreateEntity();
	auto cameraTransformComponent = std::make_unique<TransformComponent>();
	auto cameraTransformPtr = cameraTransformComponent.get();
	cameraTransformComponent->Translate(XMVectorSet(0.0f, 0.0f, -4.f, 0.0f));
	auto cameraComponent = std::make_unique<CameraComponent>(
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),
		0.4f * 3.14f, static_cast<float>(graphics->GetWidth()) / graphics->GetHeight(), 1.f, 1000.0f);
	ecsWorld->AddComponent(cameraEntity->GetID(), std::move(cameraTransformComponent));
	ecsWorld->AddComponent(cameraEntity->GetID(), std::move(cameraComponent));

	//Entity* rectEntity1 = ecsWorld->CreateEntity();
	//auto meshComponent = std::make_unique<MeshComponent>(Model("../Assets/Models/Chicken/Chicken_01.obj"));
	//auto transformComponent = std::make_unique<TransformComponent>();
	//transformComponent->Translate(XMVectorSet(0, -10, 0, 1.0f));
	//transformComponent->Scale(XMVectorSet(.1f, .1f, .1f, 1.0f));
	//ecsWorld->AddComponent(rectEntity1->GetID(), std::move(meshComponent));
	//ecsWorld->AddComponent(rectEntity1->GetID(), std::move(transformComponent));

	Entity* rectEntity2 = ecsWorld->CreateEntity();
	auto meshComponent2 = std::make_unique<MeshComponent>(Model("../Assets/Models/BoxOfCigars/model.obj"));
	auto transformComponent2 = std::make_unique<TransformComponent>();
	ecsWorld->AddComponent(rectEntity2->GetID(), std::move(meshComponent2));
	ecsWorld->AddComponent(rectEntity2->GetID(), std::move(transformComponent2));

	//Entity* rectEntity3 = ecsWorld->CreateEntity();
	//Material* mat3 = graphics->RegisterMaterial(std::make_unique<SolidMaterial>(XMFLOAT4(1,0,0,1)));
	//auto meshComponent3 = std::make_unique<MeshComponent>(MeshData(PrimitiveType::Cube), mat3);
	//auto transformComponent3 = std::make_unique<TransformComponent>();
	//transformComponent3->Translate(XMVectorSet(.0f, .0f, 5.0f, 0.0f));
	//ecsWorld->AddComponent(rectEntity3->GetID(), std::move(meshComponent3));
	//ecsWorld->AddComponent(rectEntity3->GetID(), std::move(transformComponent3));

	//Entity* skyboxEntity = ecsWorld->CreateEntity();
	//Material* matSkybox = graphics->RegisterMaterial(
	//	std::make_unique<SkyboxMaterial>("../Assets/Textures/YokotamaCitySkybox.dds", cameraTransformPtr));
	//auto meshSkybox = std::make_unique<MeshComponent>(MeshData(PrimitiveType::Cube, true), matSkybox);
	//auto transformSkybox = std::make_unique<TransformComponent>();
	//ecsWorld->AddComponent(skyboxEntity->GetID(), std::move(meshSkybox));
	//ecsWorld->AddComponent(skyboxEntity->GetID(), std::move(transformSkybox));
}

void GameWorld::Update()
{
	ECSWorld::getInstance().Update();
}
