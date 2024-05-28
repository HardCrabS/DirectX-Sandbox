#pragma once
#include "Logger.h"

class InputManager
{
	bool leftIsDown = false;
	bool rightIsDown = false;
	int x = 0;
	int y = 0;
	int rawX = 0;
	int rawY = 0;
	int mouseWheelLastDirection = 0;
private:
	InputManager() {}
public:
	static InputManager& getInstance()
	{
		static InputManager instance;
		return instance;
	}
	InputManager(const InputManager&) = delete;
	void operator=(const InputManager&) = delete;

	void ClearDelta() { rawX = 0; rawY = 0; mouseWheelLastDirection = 0; }

	void OnLeftPressed(int x, int y) { leftIsDown = true; }
	void OnRightPressed(int x, int y) { rightIsDown = true; }
	void OnLeftReleased(int x, int y) { leftIsDown = false; }
	void OnRightReleased(int x, int y) { rightIsDown = false; }
	void OnMouseMove(int x, int y) { rawX = x - this->x; rawY = this->y - y; this->x = x; this->y = y; }
	void OnMouseWheel(int x, int y, int direction) { mouseWheelLastDirection = direction; }

	bool IsLeftDown() { return leftIsDown; }
	bool IsRightDown() { return rightIsDown; }
	int GetRawX() { return rawX; }
	int GetRawY() { return rawY; }
	int GetLastMouseWheelDirection() { return mouseWheelLastDirection; }
};