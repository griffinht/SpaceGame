#include "Keyboard.h"

Keyboard::Event::Event(Type type, WPARAM key, LPARAM lParam)
{
	this->type = type;
	this->key = (unsigned char)key;
	this->lParam = lParam;
}

Keyboard::Event::Type Keyboard::Event::GetType()
{
	return type;
}

unsigned char Keyboard::Event::GetKey()
{
	return key;
}

LPARAM Keyboard::Event::GetLParam()
{
	return lParam;
}

bool Keyboard::KeyPressed(unsigned char key)
{
	return keysPressed[key];
}

std::optional<Keyboard::Event> Keyboard::GetEvent()
{
	if (events.empty())
	{
		return std::optional<Keyboard::Event>();
	}
	else
	{
		return std::optional<Keyboard::Event>(events.front());
		events.pop();
	}
}

void Keyboard::OnEvent(Keyboard::Event::Type type, WPARAM wParam, LPARAM lParam)
{
	switch (type)
	{
	case Event::Type::Keydown:
		keysPressed[wParam] = true;
		break;
	case Event::Type::Keyup:
		keysPressed[wParam] = false;
		break;
	case Event::Type::Char:
		//todo
		break;
	}

	events.push(Keyboard::Event::Event(type, wParam, lParam));

	while (maxBufferSize > 0 && events.size() > maxBufferSize)
	{
		events.pop();
	}
}
