#include <vector>
#include "EditorBaseView.h"
#include "EditorEventDispatcher.h"
#include "Constants.h"

class AssetBrowserEditor : EditorBaseView
{
	std::string currentPath;
	std::vector<std::string> items;

public:
	AssetBrowserEditor();
	void Draw() override;

private:
	std::vector<std::string> GetFilesAndDirectories(const std::string& path);
};