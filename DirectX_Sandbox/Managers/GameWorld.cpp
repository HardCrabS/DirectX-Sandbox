#include "GameWorld.h"
#include "ECS/MeshComponent.h"
#include "MeshData.h"
#include "Materials/SurfaceMaterial.h"
#include "Materials/SolidMaterial.h"
#include "Materials/SkyboxMaterial.h"
#include "ECS/CameraComponent.h"
#include "ECS/TransformComponent.h"
#include "ECS/DirectionalLightComponent.h"
#include "Model.h"
#include "SceneSaveLoad.h"

void GameWorld::Initialize()
{
	InitScene();
	SubscribeToEvents();
}

void GameWorld::Update()
{
	ECSWorld::getInstance().Update();
}

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
		0.4f * 3.14f, static_cast<float>(graphics->GetWidth()) / graphics->GetHeight(), 0.1f, 1000.0f
	);
	cameraEntity->AddComponent<OrbitCameraComponent>();
	cameraEntity->AddComponent<FreeCameraComponent>();
	cameraEntity->AddComponent<ActiveCameraComponent>();

	Entity* directionalLight = ecsWorld->CreateEntity();
	directionalLight->AddComponent<DirectionalLightComponent>(XMVectorSet(1, -1, 1, 0), XMFLOAT3(1, 1, 1), 1.f);

	SceneSaveLoad::Load();

	Entity* skyboxEntity = ecsWorld->CreateEntity();
	std::string skyboxTextureName = ASSETS_FOLDER + TEXTURES_FOLDER + "YokotamaCitySkybox.dds";
	auto shaderResource = graphics->GetResourcesContainer().GetTexture(skyboxTextureName, nullptr, TextureFormat::DDS);
	auto matSkybox = std::make_shared<SkyboxMaterial>(shaderResource, cameraTransformComponent);
	graphics->GetResourcesContainer().RegisterMaterial(skyboxTextureName, matSkybox);

	std::vector<MeshData> invertedBox = { MeshData(PrimitiveType::Cube, true) };
	std::vector<std::shared_ptr<Material>> skyMaterials = { matSkybox };
	auto meshSkybox = skyboxEntity->AddComponent<MeshComponent>(Model(invertedBox, skyMaterials));
	meshSkybox->isExcludedFromRaycast = true;
	skyboxEntity->AddComponent<TransformComponent>();
}

void GameWorld::SubscribeToEvents()
{
	auto editorEvents = &EditorEventDispatcher::getInstance();
	editorEvents->OnInstantiateEntityByAssetRequest.Subscribe(std::bind(&GameWorld::HandleAssetCreateRequest, this, std::placeholders::_1));
}

void GameWorld::HandleAssetCreateRequest(std::string assetName)
{
	logInfo("[GameWorld] HandleAssetCreateRequest: " + assetName);

	Entity* entity = ECSWorld::getInstance().CreateEntity();
	entity->SetName(assetName);
	entity->AddComponent<MeshComponent>(Model(assetName));
	entity->AddComponent<TransformComponent>();
}