#include "Mouse.h"

Mouse::Event::Event(Type type, WPARAM wParam, LPARAM lParam)
{
	this->type = type;
	this->wParam = wParam;
	this->points = MAKEPOINTS(lParam);
}

Mouse::Event::Type Mouse::Event::getType()
{
	return type;
}

POINTS Mouse::Event::getPoints()
{
	return points;
}

WPARAM Mouse::Event::getWParam()
{
	return wParam;
}

std::optional<Mouse::Event> Mouse::GetEvent()
{
	if (events.empty())
	{
		return std::optional<Mouse::Event>();
	}
	else
	{
		return std::optional<Mouse::Event>(events.front());
		events.pop();
	}
}

void Mouse::OnEvent(Mouse::Event::Type type, WPARAM wParam, LPARAM lParam)
{
	events.push(Mouse::Event::Event(type, wParam, lParam));
}

POINTS Mouse::GetMousePosition()
{
	return mousePosition;
}

POINTS Mouse::GetMousePositionDelta()
{
	POINTS d = mousePositionDelta;
	mousePosition = POINTS({ 0, 0 });
	return d;
}

int Mouse::GetMouseWheelDelta()
{
	int d = mouseWheelDelta;
	mouseWheelDelta = 0;
	return d;
}

bool Mouse::LButtonPressed()
{
	return lButtonPressed;
}

bool Mouse::MButtonPressed()
{
	return mButtonPressed;
}

bool Mouse::RButtonPressed()
{
	return rButtonPressed;
}