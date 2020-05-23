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
	void SetPos(POINTS pt);
	POINTS GetPosDelta();
	int GetWheelDelta();
	void SetWheelDelta(int delta);
	bool LButtonPressed();
	void SetLButtonPressed(bool pressed);
	bool MButtonPressed();
	void SetMButtonPressed(bool pressed);
	bool RButtonPressed();
	void SetRButtonPressed(bool pressed);
	bool Constrained();
	void SetConstrained(bool constrained);
private:
	std::optional<Mouse::Event> GetEvent();
	POINTS mousePosition;
	POINTS mousePositionDelta;
	bool constrained = false;
	int mouseWheelDelta;
	bool lButtonPressed;
	bool mButtonPressed;
	bool rButtonPressed;
	std::queue<Mouse::Event> events;
	const int maxBufferSize = -1;
};