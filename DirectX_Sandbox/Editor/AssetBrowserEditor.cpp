#include "AssetBrowserEditor.h"
#include <filesystem>

namespace fs = std::filesystem;

AssetBrowserEditor::AssetBrowserEditor()
{
    currentPath = ASSETS_FOLDER;
    items = GetFilesAndDirectories(currentPath);
}

void AssetBrowserEditor::Draw()
{
    ImGui::Begin("File Explorer");
    ImGui::Text("Current Path: %s", currentPath.c_str());

    if (ImGui::Button("Up")) {
        try {
            currentPath = fs::path(currentPath).parent_path().string();
            items = GetFilesAndDirectories(currentPath);
        }
        catch (const std::filesystem::filesystem_error& e) {
            logError("[AssetBrowserEditor] Filesystem error: " + std::string(e.what()));
        }
        catch (const std::exception& e) {
            logError("[AssetBrowserEditor] General exception: " + std::string(e.what()));
        }
    }

    for (const auto& item : items) {
        try {
            if (fs::is_directory(item)) {
                if (ImGui::Selectable(("[Dir] " + fs::path(item).filename().string()).c_str())) {
                    currentPath = item;
                    items = GetFilesAndDirectories(currentPath);
                    break;
                }
            }
            else {
                ImGui::Selectable(fs::path(item).filename().string().c_str());
                if (ImGui::IsItemClicked()) {
                    EditorEventDispatcher::getInstance().InstantiateEntityByAsset(item);
                }
            }
        }
        catch (const std::filesystem::filesystem_error& e) {
            logError("[AssetBrowserEditor] Filesystem error: " + std::string(e.what()));
        }
        catch (const std::exception& e) {
            logError("[AssetBrowserEditor] General exception: " + std::string(e.what()));
        }
    }

    ImGui::End();
}

std::vector<std::string> AssetBrowserEditor::GetFilesAndDirectories(const std::string& path) {
    std::vector<std::string> items;
    for (const auto& entry : fs::directory_iterator(path)) {
        items.push_back(entry.path().string());
    }
    return items;
}