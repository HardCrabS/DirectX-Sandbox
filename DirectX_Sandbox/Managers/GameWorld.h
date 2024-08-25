#pragma once
#include <memory>
#include "ECSWorld.h"
#include "Graphics.h"
#include "Editor/EditorEventDispatcher.h"

class GameWorld
{
public:
	void Initialize();
	void Update();

private:
	void InitScene();
	void SubscribeToEvents();

	void HandleAssetCreateRequest(std::string assetName);
};