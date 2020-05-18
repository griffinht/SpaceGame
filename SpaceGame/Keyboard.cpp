#include "Keyboard.h"

Keyboard::Event::Event(Type type, WPARAM wParam, LPARAM lParam)
{
	this->type = type;
	this->wParam = wParam;
	this->points = MAKEPOINTS(lParam);
}

Keyboard::Event::Type Keyboard::Event::getType()
{
	return type;
}

POINTS Keyboard::Event::getPoints()
{
	return points;
}

WPARAM Keyboard::Event::getWParam()
{
	return wParam;
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
	events.push(Keyboard::Event::Event(type, wParam, lParam));
}
