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
		POINTS GetPos();
		int GetPosX();
		int GetPosY();
		WPARAM getWParam();
	private:
		Type type;
		POINTS mousePosition;
		WPARAM wParam;
	};
public:
	void OnEvent(Mouse::Event::Type type, WPARAM wParam, LPARAM lParam);
	POINTS GetPos();
	int GetPosX();
	int GetPosY();
	POINTS GetPosDelta();
	int GetPosDeltaX();
	int GetPosDeltaY();
	int GetWheelDelta();
	bool LButtonPressed();
	bool MButtonPressed();
	bool RButtonPressed();
private:
	std::optional<Mouse::Event> GetEvent();
	POINTS mousePosition;
	POINTS mousePositionDelta;
	int sdf;
	int mouseWheelDelta;
	bool lButtonPressed;
	bool mButtonPressed;
	bool rButtonPressed;
	std::queue<Mouse::Event> events;
	const int maxBufferSize = -1;
};