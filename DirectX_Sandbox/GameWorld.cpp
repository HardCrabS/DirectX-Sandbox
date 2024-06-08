#include "GameWorld.h"
#include "MeshComponent.h"
#include "MeshData.h"
#include "SurfaceMaterial.h"
#include "SolidMaterial.h"
#include "SkyboxMaterial.h"
#include "CameraComponent.h"
#include "TransformComponent.h"

void GameWorld::InitScene()
{
	ECSWorld* ecsWorld = &ECSWorld::getInstance();
	ecsWorld->Initialize();

	Entity* cameraEntity = ecsWorld->CreateEntity();
	auto cameraTransformComponent = std::make_unique<TransformComponent>();
	auto cameraTransformPtr = cameraTransformComponent.get();
	cameraTransformComponent->Translate(DirectX::XMVectorSet(0.0f, 0.0f, -4.f, 0.0f));
	auto cameraComponent = std::make_unique<CameraComponent>(
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),
		0.4f * 3.14f, (float)Graphics::getInstance().GetWidth() / Graphics::getInstance().GetHeight(), 1.f, 1000.0f);
	ecsWorld->AddComponent(cameraEntity->GetID(), std::move(cameraTransformComponent));
	ecsWorld->AddComponent(cameraEntity->GetID(), std::move(cameraComponent));

	Entity* rectEntity1 = ecsWorld->CreateEntity();
	Material* mat1 = Graphics::getInstance().RegisterMaterial(
		std::make_unique<SurfaceMaterial>(L"../Assets/Textures/dog.jpg"));
	auto meshComponent = std::make_unique<MeshComponent>(MeshData("../Assets/Models/TestMaterial1.fbx"), mat1);
	auto transformComponent = std::make_unique<TransformComponent>();
	transformComponent->Scale(DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
	ecsWorld->AddComponent(rectEntity1->GetID(), std::move(meshComponent));
	ecsWorld->AddComponent(rectEntity1->GetID(), std::move(transformComponent));

	Entity* rectEntity2 = ecsWorld->CreateEntity();
	Material* mat2 = Graphics::getInstance().RegisterMaterial(
		std::make_unique<SurfaceMaterial>(L"../Assets/Textures/cat.jpg"));
	auto meshComponent2 = std::make_unique<MeshComponent>(MeshData("../Assets/Models/TestSimpleCube.obj"), mat2);
	auto transformComponent2 = std::make_unique<TransformComponent>();
	transformComponent2->Translate(DirectX::XMVectorSet(-4.0f, .0f, 0.0f, 0.0f));
	ecsWorld->AddComponent(rectEntity2->GetID(), std::move(meshComponent2));
	ecsWorld->AddComponent(rectEntity2->GetID(), std::move(transformComponent2));

	Entity* rectEntity3 = ecsWorld->CreateEntity();
	Material* mat3 = Graphics::getInstance().RegisterMaterial(std::make_unique<SolidMaterial>(DirectX::XMFLOAT4(1,0,0,1)));
	auto meshComponent3 = std::make_unique<MeshComponent>(MeshData(PrimitiveType::Cube), mat3);
	auto transformComponent3 = std::make_unique<TransformComponent>();
	transformComponent3->Translate(DirectX::XMVectorSet(.0f, .0f, 5.0f, 0.0f));
	ecsWorld->AddComponent(rectEntity3->GetID(), std::move(meshComponent3));
	ecsWorld->AddComponent(rectEntity3->GetID(), std::move(transformComponent3));

	Entity* skyboxEntity = ecsWorld->CreateEntity();
	Material* matSkybox = Graphics::getInstance().RegisterMaterial(
		std::make_unique<SkyboxMaterial>(L"../Assets/Textures/daysky_skybox.dds", cameraTransformPtr));
	auto meshSkybox = std::make_unique<MeshComponent>(MeshData(PrimitiveType::Cube, true), matSkybox);
	auto transformSkybox = std::make_unique<TransformComponent>();
	ecsWorld->AddComponent(skyboxEntity->GetID(), std::move(meshSkybox));
	ecsWorld->AddComponent(skyboxEntity->GetID(), std::move(transformSkybox));
}

void GameWorld::Update()
{
	ECSWorld::getInstance().Update();
}
