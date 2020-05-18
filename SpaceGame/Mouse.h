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
		std::pair<int, int> GetPos();
		int GetPosX();
		int GetPosY();
		WPARAM getWParam();
	private:
		Type type;
		std::pair<int, int> mousePosition;
		WPARAM wParam;
	};
public:
	Mouse();
	void OnEvent(Mouse::Event::Type type, WPARAM wParam, LPARAM lParam);
	std::pair<int, int> GetPos();
	int GetPosX();
	int GetPosY();
	std::pair<int, int> GetPosDelta();
	int GetPosDeltaX();
	int GetPosDeltaY();
	int GetWheelDelta();
	bool LButtonPressed();
	bool MButtonPressed();
	bool RButtonPressed();
private:
	std::optional<Mouse::Event> GetEvent();
	std::pair<int, int> mousePosition;
	std::pair<int, int> mousePositionDelta;
	int mouseWheelDelta;
	bool lButtonPressed;
	bool mButtonPressed;
	bool rButtonPressed;
	std::queue<Mouse::Event> events;
	const int maxBufferSize = -1;
private:
	static std::pair<int, int> AddPositions(std::pair<int, int> p1, std::pair<int, int> p2);
	static std::pair<int, int> ToPair(POINTS pt);
};