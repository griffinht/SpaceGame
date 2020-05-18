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
			MouseWheel,
			RButtonDown,
			RButtonUp
		};
	public:
		Event(Type type, WPARAM wParam, LPARAM lParam);
		Type getType();
		POINTS getPoints();
		WPARAM getWParam();
	private:
		Type type;
		POINTS points;
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
	POINTS mousePosition;
	POINTS mousePositionDelta;
	int mouseWheelDelta;
	bool lButtonPressed;
	bool mButtonPressed;
	bool rButtonPressed;
private:
	std::queue<Mouse::Event> events;
};