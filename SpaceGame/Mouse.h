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
		WPARAM getWParam();
	private:
		Type type;
		POINTS mousePosition;
		WPARAM wParam;
	};
public:
	Mouse();
	void OnEvent(Mouse::Event::Type type, WPARAM wParam, LPARAM lParam);
	void OnRawEvent(tagRAWMOUSE rawMouse);
	POINTS GetPos();
	POINTS GetPosDelta();
	int GetWheelDelta();
	bool LButtonPressed();
	bool MButtonPressed();
	bool RButtonPressed();
	void SetRawInput(bool rawInput);
	void SetConstrained(bool constrain);
	void SetCenter(POINTS c);
private:
	std::optional<Mouse::Event> GetEvent();
	POINTS position;
	POINTS positionDelta;
	bool constrained = false;
	bool rawInput = false;
	int wheelDelta;
	bool lButtonPressed;
	bool mButtonPressed;
	bool rButtonPressed;
	std::queue<Mouse::Event> events;
	const int maxBufferSize = -1;
	POINTS center;
};