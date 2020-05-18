#include "Mouse.h"

Mouse::Event::Event(Type type, WPARAM wParam, LPARAM lParam)
{
	this->type = type;
	this->mousePosition = MAKEPOINTS(lParam);
	this->wParam = wParam
}

Mouse::Event::Type Mouse::Event::getType()
{
	return type;
}

POINTS Mouse::Event::GetMousePos()
{
	return POINTS();
}

float Mouse::Event::GetMousePosX()
{
	return mousePosition.x;
}

float Mouse::Event::GetMousePosY()
{
	return mousePosition.y;
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

POINTS Mouse::AddPoints(POINTS p1, POINTS p2)
{
	return POINTS({ p1.x + p2.x, p1.y + p2.y });
}

void Mouse::OnEvent(Mouse::Event::Type type, WPARAM wParam, LPARAM lParam)
{
	switch (type)
	{
	case Mouse::Event::Type::Move:
		POINTS pt = MAKEPOINTS(lParam);
		mousePosition = AddPoints(mousePosition, pt);
		mousePositionDelta = AddPoints(mousePositionDelta, pt);
		break;
	case Mouse::Event::Type::LButtonDown:
		lButtonPressed = true;
		break;
	case Mouse::Event::Type::LButtonUp:
		lButtonPressed = false;
		break;
	case Mouse::Event::Type::MButtonDown:
		mButtonPressed = true;
		break;
	case Mouse::Event::Type::MButtonUp:
		mButtonPressed = false;
		break;
	case Mouse::Event::Type::RButtonDown:
		rButtonPressed = true;
		break;
	case Mouse::Event::Type::RButtonUp:
		rButtonPressed = false;
		break;
	case Mouse::Event::Type::MouseWheel:
		mouseWheelDelta += GET_WHEEL_DELTA_WPARAM(wParam);
		break;
	}
	events.push(Mouse::Event::Event(type, wParam, lParam));
	while (maxBufferSize > 0 && events.size() > maxBufferSize)
	{
		events.pop();
	}
}

POINTS Mouse::GetMousePos()
{
	return mousePosition;
}

float Mouse::GetMousePosX()
{
	return mousePosition.x;
}

float Mouse::GetMousePosY()
{
	return mousePosition.y;
}

POINTS Mouse::GetMousePosDelta()
{
	POINTS d = mousePositionDelta;
	mousePositionDelta = POINTS({ 0, 0 });
	return d;
}

float Mouse::GetMousePosDeltaX()
{
	return mousePositionDelta.x;
}

float Mouse::GetMousePosDeltaY()
{
	return mousePositionDelta.y;
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