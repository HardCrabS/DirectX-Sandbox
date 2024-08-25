#pragma once	
#include "string"
#include "Event.h"
#include "Managers/Singleton.h"

class EditorEventDispatcher : public Singleton<EditorEventDispatcher>
{
	friend class Singleton<EditorEventDispatcher>;
private:
	EditorEventDispatcher() {}

public:
	Event<std::string> OnInstantiateEntityByAssetRequest;

public:
	void InstantiateEntityByAsset(std::string assetName)
	{
		logInfo("[EditorEventDispatcher] OnAssetCreateRequest: " + assetName);
		OnInstantiateEntityByAssetRequest.Notify(assetName);
	}
};