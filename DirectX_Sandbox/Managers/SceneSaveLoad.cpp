#include "SceneSaveLoad.h"

void SceneSaveLoad::Save()
{
    logInfo("[SceneSaveLoad] Save");
    std::ofstream file(SCENE_FILE);
    if (!file.is_open()) {
        logError("[SceneSaveLoad] Could not open file for saving: " + SCENE_FILE);
        return;
    }

    const auto& entities = ECSWorld::getInstance().GetEntities();

    for (const auto& it : entities) {
        auto& e = it.second;

        // TODO: load any entity
        if (!e->HasComponents(MeshComponent::TypeID()))
            continue;

        auto mesh = e->GetComponent<MeshComponent>();

        if (mesh->GetModel().GetFilename().empty())
            continue;

        logInfo("[SceneSaveLoad] Saving entity: " + mesh->GetModel().GetFilename());

        file << mesh->GetModel().GetFilename() << std::endl;

        auto transform = e->GetComponent<TransformComponent>();
        VectorToStream(file, transform->GetPosition());
        VectorToStream(file, transform->GetRotation());
        VectorToStream(file, transform->GetScale());
    }

    file.close();
}

void SceneSaveLoad::Load()
{
    logInfo("[SceneSaveLoad] Load");
    std::ifstream file(SCENE_FILE);
    if (!file.is_open()) {
        logError("[SceneSaveLoad] Could not open file for loading: " + SCENE_FILE);
        return;
    }

    std::string modelFilename;
    float px, py, pz, pw;
    float rx, ry, rz, rw;
    float sx, sy, sz, sw;

    while (file >> modelFilename) {
        file >> px >> py >> pz >> pw;
        XMVECTOR position = XMVectorSet(px, py, pz, pw);

        file >> rx >> ry >> rz >> rw;
        XMVECTOR rotation = XMVectorSet(rx, ry, rz, rw);

        file >> sx >> sy >> sz >> sw;
        XMVECTOR scale = XMVectorSet(sx, sy, sz, sw);

        CreateEntity(modelFilename, position, rotation, scale);
    }

    file.close();
}

void SceneSaveLoad::CreateEntity(const std::string& filename, const XMVECTOR& pos, const XMVECTOR& rot, const XMVECTOR& scale)
{
    Entity* entity = ECSWorld::getInstance().CreateEntity();
    entity->SetName(filename);
    entity->AddComponent<MeshComponent>(Model(filename));
    auto transform = entity->AddComponent<TransformComponent>();
    transform->SetPosition(pos);
    transform->SetRotation(rot);
    transform->SetScale(scale);
}

void SceneSaveLoad::VectorToStream(std::ofstream& fs, const XMVECTOR& vector)
{
    fs << XMVectorGetX(vector) << " "
        << XMVectorGetY(vector) << " "
        << XMVectorGetZ(vector) << " "
        << XMVectorGetW(vector) << std::endl;
}
