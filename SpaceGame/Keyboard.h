#pragma once

#include <Windows.h>
#include <optional>
#include <queue>

class Keyboard
{
public:
	class Event
	{
	public:
		enum class Type
		{
			Keydown,
			Keyup,
			Char,
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
	Keyboard() = default;
	std::optional<Keyboard::Event> GetEvent();
	void OnEvent(Keyboard::Event::Type type, WPARAM wParam, LPARAM lParam);
private:
	std::queue<Keyboard::Event> events;
	const int maxBufferSize = 64;
};