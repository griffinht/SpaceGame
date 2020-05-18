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
		POINTS getMousePosition();
		WPARAM getWParam();
	private:
		Type type;
		POINTS mousePosition;
		WPARAM wParam;
	};
public:
	Mouse() = default;
	void OnEvent(Mouse::Event::Type type, WPARAM wParam, LPARAM lParam);
	POINTS GetMousePosition();
	POINTS GetMousePositionDelta();
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