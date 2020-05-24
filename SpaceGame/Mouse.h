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
	void OnEvent(Mouse::Event::Type type, WPARAM wParam, LPARAM lParam);
	POINTS GetPos();
	POINTS GetPosDelta();
	int GetWheelDelta();
	bool LButtonPressed();
	bool MButtonPressed();
	bool RButtonPressed();
	bool Constrained();
	void Constrain(bool constrain);
	void SetSize(int width, int height);
private:
	std::optional<Mouse::Event> GetEvent();
	POINTS position;
	POINTS positionDelta;
	bool constrained = false;
	int wheelDelta;
	bool lButtonPressed;
	bool mButtonPressed;
	bool rButtonPressed;
	std::queue<Mouse::Event> events;
	const int maxBufferSize = -1;
	POINTS center;
};