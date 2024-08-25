#pragma once
#include "Logger.h"
#include "Event.h"

class InputManager
{
	bool leftIsDown = false;
	bool rightIsDown = false;
	int x = 0;
	int y = 0;
	int rawX = 0;
	int rawY = 0;
	int mouseWheelLastDirection = 0;
	bool keyStates[256];

public:
	Event<int, int> OnLeftMouseButtonClick;
	Event<const unsigned char> OnKeyPressedEvent;

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

	// mouse
	void OnLeftPressed(int x, int y) { 
		leftIsDown = true; 
		OnLeftMouseButtonClick.Notify(x, y);
	}
	void OnRightPressed(int x, int y) { rightIsDown = true; }
	void OnLeftReleased(int x, int y) { leftIsDown = false; }
	void OnRightReleased(int x, int y) { rightIsDown = false; }
	void OnMouseMove(int x, int y) { rawX = x - this->x; rawY = this->y - y; this->x = x; this->y = y; }
	void OnMouseWheel(int x, int y, int direction) { mouseWheelLastDirection = direction; }

	// keyboard
	bool IsKeyPressed(const unsigned char keycode) { return this->keyStates[keycode]; }
	void OnKeyPressed(const unsigned char key) { 
		this->keyStates[key] = true; 
		OnKeyPressedEvent.Notify(key);
	}
	void OnKeyReleased(const unsigned char key) { this->keyStates[key] = false; }

	bool IsLeftDown() { return leftIsDown; }
	bool IsRightDown() { return rightIsDown; }
	int GetRawX() { return rawX; }
	int GetRawY() { return rawY; }
	int GetLastMouseWheelDirection() { return mouseWheelLastDirection; }
};