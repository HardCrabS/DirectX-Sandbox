#include "Singleton.h"
#include "ECSWorld.h"
#include "ECS/Entity.h"
#include "ECS/MeshComponent.h"
#include <vector>
#include <fstream>
#include <iostream>

inline const std::string SCENE_FILE = "scene.txt";

class SceneSaveLoad : public Singleton<SceneSaveLoad>
{
	friend class Singleton<SceneSaveLoad>;
private:
	SceneSaveLoad() {}

public:
	static void Save();
	static void Load();
	static void CreateEntity(const std::string& filename, const XMVECTOR& pos, const XMVECTOR& rot, const XMVECTOR& scale);
private:
	static void VectorToStream(std::ofstream& fs, const XMVECTOR& vector);
};