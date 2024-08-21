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
	auto cameraTransformComponent = cameraEntity->AddComponent<TransformComponent>();
	cameraTransformComponent->Translate(XMVectorSet(0.0f, 0.0f, -4.f, 0.0f));
	cameraEntity->AddComponent<CameraComponent>(
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),
		0.4f * 3.14f, static_cast<float>(graphics->GetWidth()) / graphics->GetHeight(), 1.f, 1000.0f
	);
	cameraEntity->AddComponent<OrbitComponent>();
	cameraEntity->AddComponent<ActiveCameraComponent>();

	Entity* directionalLight = ecsWorld->CreateEntity();
	directionalLight->AddComponent<DirectionalLightComponent>(XMVectorSet(1, -1, 1, 0), XMFLOAT3(1, 1, 1), 1.f);

	//Entity* directionalLight2 = ecsWorld->CreateEntity();
	//auto directLightComponent2 = std::make_unique<DirectionalLightComponent>(
	//	XMVectorSet(0, 0, 1, 0), XMFLOAT3(1, 1, 1), 1.f);
	//ecsWorld->AddComponent(directionalLight2->GetID(), std::move(directLightComponent2));

	Entity* rectEntity1 = ecsWorld->CreateEntity();
	rectEntity1->SetName("Chicken");
	rectEntity1->AddComponent<MeshComponent>(Model("../Assets/Models/Chicken/Chicken_01.obj"));
	auto transformComponent = rectEntity1->AddComponent<TransformComponent>();
	transformComponent->Translate(XMVectorSet(0, -10, 0, 1.0f));
	transformComponent->Scale(XMVectorSet(.1f, .1f, .1f, 1.0f));

	Entity* rectEntity2 = ecsWorld->CreateEntity();
	rectEntity2->SetName("Planet");
	rectEntity2->AddComponent<MeshComponent>(Model("../Assets/Models/Planet_1.fbx"));
	auto transformComponent2 = rectEntity2->AddComponent<TransformComponent>();
	transformComponent2->Translate(XMVectorSet(10, 0, 0, 1.0f));

	//Entity* rectEntity3 = ecsWorld->CreateEntity();
	//Material* mat3 = graphics->RegisterMaterial(std::make_unique<SolidMaterial>(XMFLOAT4(1,0,0,1)));
	//auto meshComponent3 = std::make_unique<MeshComponent>(MeshData(PrimitiveType::Cube), mat3);
	//auto transformComponent3 = std::make_unique<TransformComponent>();
	//transformComponent3->Translate(XMVectorSet(.0f, .0f, 5.0f, 0.0f));
	//ecsWorld->AddComponent(rectEntity3->GetID(), std::move(meshComponent3));
	//ecsWorld->AddComponent(rectEntity3->GetID(), std::move(transformComponent3));








	//Entity* bookEntity1 = ecsWorld->CreateEntity();
	//bookEntity1->SetName("LittleBookcase");
	//auto bookmeshComponent = std::make_unique<MeshComponent>(Model("../Assets/Models/LittleBookcase/model.obj"));
	//auto booktransformComponent = std::make_unique<TransformComponent>();
	//booktransformComponent->Translate(XMVectorSet(0, 0, 0, 1.0f));
	//booktransformComponent->Scale(XMVectorSet(3, 3, 3, 1.0f));
	//bookEntity1->AddComponent(std::move(bookmeshComponent));
	//bookEntity1->AddComponent(std::move(booktransformComponent));

	Entity* skyboxEntity = ecsWorld->CreateEntity();
	std::string skyboxPath = "../Assets/Textures/YokotamaCitySkybox.dds";
	auto shaderResource = graphics->GetResourcesContainer().GetTexture(skyboxPath, nullptr, TextureFormat::DDS);
	auto matSkybox = std::make_shared<SkyboxMaterial>(shaderResource, cameraTransformComponent);
	graphics->GetResourcesContainer().RegisterMaterial(skyboxPath, matSkybox);

	std::vector<MeshData> invertedBox = { MeshData(PrimitiveType::Cube, true) };
	std::vector<std::shared_ptr<Material>> skyMaterials = { matSkybox };
	auto meshSkybox = skyboxEntity->AddComponent<MeshComponent>(Model(invertedBox, skyMaterials));
	meshSkybox->isExcludedFromRaycast = true;
	skyboxEntity->AddComponent<TransformComponent>();
}

void GameWorld::Update()
{
	ECSWorld::getInstance().Update();
}
