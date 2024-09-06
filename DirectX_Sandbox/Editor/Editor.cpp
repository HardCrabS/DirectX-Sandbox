#include "Editor.h"

void Editor::Draw()
{
	assetBrowser.Draw();

	ImGui::Begin("Controls");
	if (ImGui::Button("Save")) {
		SceneSaveLoad::Save();
	}
	ImGui::End();
}