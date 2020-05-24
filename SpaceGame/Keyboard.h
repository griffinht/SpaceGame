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
	bool KeyPressed(unsigned char key);
	std::optional<Keyboard::Event> GetEvent();
	void OnEvent(Keyboard::Event::Type type, WPARAM wParam, LPARAM lParam);
private:
	std::queue<Keyboard::Event> events;
	bool keysPressed[256] = { false };
	const int maxBufferSize = 64;
};