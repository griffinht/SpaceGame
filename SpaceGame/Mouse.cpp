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
	switch (type)
	{
	case Event::Type::Move:
		if (constrained)
		{
			POINT pt;
			GetCursorPos(&pt);
			positionDelta = { positionDelta.x + (short) pt.x - center.x, positionDelta.y + (short) pt.y - center.y };
			SetCursorPos(center.x, center.y);
		}
		else
		{
			POINTS pt = MAKEPOINTS(lParam);
			positionDelta = { positionDelta.x + pt.x - position.x, positionDelta.y + pt.y - position.y };
			position = pt;
		}
		break;
	case Event::Type::LButtonDown:
		lButtonPressed = false;
		break;
	case Event::Type::LButtonUp:
		lButtonPressed = true;
		break;
	case Event::Type::MButtonDown:
		mButtonPressed = true;
		break;
	case Event::Type::MButtonUp:
		mButtonPressed = false;
		break;
	case Event::Type::RButtonDown:
		Constrain(true);
		rButtonPressed = true;
		break;
	case Event::Type::RButtonUp:
		Constrain(false);
		rButtonPressed = false;
		break;
	case Event::Type::MouseWheel:
		wheelDelta += GET_WHEEL_DELTA_WPARAM(wParam);
		break;
	}
	events.push(Mouse::Event::Event(type, wParam, lParam));
	while (maxBufferSize > 0 && events.size() > maxBufferSize)
	{
		events.pop();
	}
}

POINTS Mouse::GetPos()
{
	return position;
}

POINTS Mouse::GetPosDelta()
{
	POINTS d = positionDelta;
	positionDelta = { 0, 0 };
	return d;
}

int Mouse::GetWheelDelta()
{
	int d = wheelDelta;
	wheelDelta = 0;
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

bool Mouse::Constrained()
{
	return constrained;
}

void Mouse::Constrain(bool constrain)
{
	constrained = constrain;
	ShowCursor(!constrain);
}

void Mouse::SetCenter(POINTS c)
{
	center = c;
}