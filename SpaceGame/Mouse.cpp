#include "Mouse.h"
#include <string>
#include <sstream>

Mouse::Event::Event(Type type, WPARAM wParam, LPARAM lParam)
{
	this->type = type;
	mousePosition = MAKEPOINTS(lParam);
	this->wParam = wParam;
}

Mouse::Event::Type Mouse::Event::getType()
{
	return type;
}

POINTS Mouse::Event::GetPos()
{
	return mousePosition;
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
	while (maxBufferSize > 0 && events.size() > maxBufferSize)
	{
		events.pop();
	}
}

POINTS Mouse::GetPos()
{
	return mousePosition;
}

void Mouse::SetPos(POINTS pt)
{
	mousePositionDelta = { mousePositionDelta.x + pt.x - mousePosition.x, mousePositionDelta.y + pt.y - mousePosition.y };
	if (constrained)
	{
		SetCursorPos(0, 0);
		mousePosition = pt;
	}
	else
	{
		mousePosition = pt;
	}
}

POINTS Mouse::GetPosDelta()
{
	POINTS d = mousePositionDelta;
	mousePositionDelta = { 0, 0 };
	return d;
}

int Mouse::GetWheelDelta()
{
	int d = mouseWheelDelta;
	mouseWheelDelta = 0;
	return d;
}

void Mouse::SetWheelDelta(int delta)
{
	mouseWheelDelta += delta;
}

bool Mouse::LButtonPressed()
{
	return lButtonPressed;
}

void Mouse::SetLButtonPressed(bool pressed)
{
	lButtonPressed = pressed;
}

bool Mouse::MButtonPressed()
{
	return mButtonPressed;
}

void Mouse::SetMButtonPressed(bool pressed)
{
	mButtonPressed = true;
}

bool Mouse::RButtonPressed()
{
	return rButtonPressed;
}

void Mouse::SetRButtonPressed(bool pressed)
{
	rButtonPressed = true;
}

void Mouse::SetConstrained(bool constrained)
{
	this->constrained = constrained;
	ShowCursor(!constrained);
}

bool Mouse::Constrained()
{
	return constrained;
}
