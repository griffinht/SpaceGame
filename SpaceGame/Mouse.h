#pragma once

#include <Windows.h>
#include <optional>
#include <queue>

class Mouse
{
public:
	class Event
	{
	public:
		enum class Type
		{
			Move,
			LButtonDown,
			LButtonUp,
			MButtonDown,
			MButtonUp,
			RButtonDown,
			RButtonUp,
			MouseWheel
		};
	public:
		Event(Type type, WPARAM wParam, LPARAM lParam);
		Type getType();
		POINTS GetMousePos();
		float GetMousePosX();
		float GetMousePosY();
		WPARAM getWParam();
	private:
		Type type;
		POINTS mousePosition;
		WPARAM wParam;
	};
public:
	Mouse() = default;
	void OnEvent(Mouse::Event::Type type, WPARAM wParam, LPARAM lParam);
	POINTS GetMousePos();
	float GetMousePosX();
	float GetMousePosY();
	POINTS GetMousePosDelta();
	float GetMousePosDeltaX();
	float GetMousePosDeltaY();
	int GetMouseWheelDelta();
	bool LButtonPressed();
	bool MButtonPressed();
	bool RButtonPressed();
private:
	std::optional<Mouse::Event> GetEvent();
	POINTS mousePosition = POINTS({0, 0});
	POINTS mousePositionDelta = POINTS({0, 0});
	int mouseWheelDelta = 0;
	bool lButtonPressed = false;
	bool mButtonPressed = false;
	bool rButtonPressed = false;
	std::queue<Mouse::Event> events;
private:
	static POINTS AddPoints(POINTS p1, POINTS p2);
};