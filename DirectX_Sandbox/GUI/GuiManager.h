#include "framework.h"
#include "Graphics.h"


class GuiManager
{
public:
	void Initialize(HWND hwnd);
	void StartFrame();
	void Render();
	void CleanUp();
};