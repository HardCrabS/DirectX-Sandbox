#include "GameWorld.h"
#include "MeshComponent.h"
#include "MeshData.h"
#include "SurfaceMaterial.h"
#include "SolidMaterial.h"
#include "SkyboxMaterial.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "DirectionalLightComponent.h"
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

	Entity* directionalLight = ecsWorld->CreateEntity();
	auto directLightComponent = std::make_unique<DirectionalLightComponent>(
		XMVectorSet(1, -1, 1, 0), XMFLOAT3(1, 1, 1), 1.f);
	ecsWorld->AddComponent(directionalLight->GetID(), std::move(directLightComponent));

	//Entity* directionalLight2 = ecsWorld->CreateEntity();
	//auto directLightComponent2 = std::make_unique<DirectionalLightComponent>(
	//	XMVectorSet(0, 0, 1, 0), XMFLOAT3(1, 1, 1), 1.f);
	//ecsWorld->AddComponent(directionalLight2->GetID(), std::move(directLightComponent2));

	Entity* rectEntity1 = ecsWorld->CreateEntity();
	auto meshComponent = std::make_unique<MeshComponent>(Model("../Assets/Models/Chicken/Chicken_01.obj"));
	auto transformComponent = std::make_unique<TransformComponent>();
	transformComponent->Translate(XMVectorSet(0, -10, 0, 1.0f));
	transformComponent->Scale(XMVectorSet(.1f, .1f, .1f, 1.0f));
	ecsWorld->AddComponent(rectEntity1->GetID(), std::move(meshComponent));
	ecsWorld->AddComponent(rectEntity1->GetID(), std::move(transformComponent));

	//Entity* rectEntity2 = ecsWorld->CreateEntity();
	//auto meshComponent2 = std::make_unique<MeshComponent>(Model("../Assets/Models/LittleBookcase/model.obj"));
	//auto transformComponent2 = std::make_unique<TransformComponent>();
	//transformComponent2->Translate(XMVectorSet(0, 0, 0, 1.0f));
	//transformComponent2->Rotate(0, 0, 0);
	//transformComponent2->Scale(XMVectorSet(3, 3, 3, 1.0f));
	//ecsWorld->AddComponent(rectEntity2->GetID(), std::move(meshComponent2));
	//ecsWorld->AddComponent(rectEntity2->GetID(), std::move(transformComponent2));

	//Entity* rectEntity3 = ecsWorld->CreateEntity();
	//Material* mat3 = graphics->RegisterMaterial(std::make_unique<SolidMaterial>(XMFLOAT4(1,0,0,1)));
	//auto meshComponent3 = std::make_unique<MeshComponent>(MeshData(PrimitiveType::Cube), mat3);
	//auto transformComponent3 = std::make_unique<TransformComponent>();
	//transformComponent3->Translate(XMVectorSet(.0f, .0f, 5.0f, 0.0f));
	//ecsWorld->AddComponent(rectEntity3->GetID(), std::move(meshComponent3));
	//ecsWorld->AddComponent(rectEntity3->GetID(), std::move(transformComponent3));

	Entity* skyboxEntity = ecsWorld->CreateEntity();
	std::string skyboxPath = "../Assets/Textures/YokotamaCitySkybox.dds";
	auto shaderResource = graphics->GetResourcesContainer().GetTexture(skyboxPath, nullptr, TextureFormat::DDS);
	auto matSkybox = std::make_shared<SkyboxMaterial>(shaderResource, cameraTransformPtr);
	graphics->GetResourcesContainer().RegisterMaterial(skyboxPath, matSkybox);

	std::vector<MeshData> invertedBox = { MeshData(PrimitiveType::Cube, true) };
	std::vector<std::shared_ptr<Material>> skyMaterials = { matSkybox };
	auto meshSkybox = std::make_unique<MeshComponent>(Model(invertedBox, skyMaterials));
	auto transformSkybox = std::make_unique<TransformComponent>();
	ecsWorld->AddComponent(skyboxEntity->GetID(), std::move(meshSkybox));
	ecsWorld->AddComponent(skyboxEntity->GetID(), std::move(transformSkybox));
}

void GameWorld::Update()
{
	ECSWorld::getInstance().Update();
}
