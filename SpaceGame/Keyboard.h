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
		Type GetType();
		unsigned char GetKey();
		LPARAM GetLParam();
	private:
		Type type;
		unsigned char key;
		LPARAM lParam;
	};
public:
	Keyboard() = default;
	std::optional<Keyboard::Event> GetEvent();
	void OnEvent(Keyboard::Event::Type type, WPARAM wParam, LPARAM lParam);
private:
	std::queue<Keyboard::Event> events;
	const int maxBufferSize = 64;
};