#include "Mouse.h"
#include <string>

Mouse::Event::Event(Type type, WPARAM wParam, LPARAM lParam)
{
	this->type = type;
	this->mousePosition = ToPair(MAKEPOINTS(lParam));
	this->wParam = wParam;
}

Mouse::Event::Type Mouse::Event::getType()
{
	return type;
}

std::pair<int, int> Mouse::Event::GetPos()
{
	return mousePosition;
}

int Mouse::Event::GetPosX()
{
	return mousePosition.first;
}

int Mouse::Event::GetPosY()
{
	return mousePosition.second;
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

std::pair<int, int> Mouse::AddPositions(std::pair<int, int> p1, std::pair<int, int> p2)
{
	return std::pair<int, int>({ p1.first + p2.first, p1.second + p2.second});
}

std::pair<int, int> Mouse::ToPair(POINTS pt)
{
	return std::pair<int, int>(pt.x, pt.y);
}

Mouse::Mouse()
	:
	mousePosition{0,0},
	mousePositionDelta{0,0},
	mouseWheelDelta(0),
	lButtonPressed(false),
	mButtonPressed(false),
	rButtonPressed(false)
{}

void Mouse::OnEvent(Mouse::Event::Type type, WPARAM wParam, LPARAM lParam)
{
	switch (type)
	{
	case Mouse::Event::Type::Move:
		{
			std::pair<int, int> pos = ToPair(MAKEPOINTS(lParam));
			mousePosition = AddPositions(mousePosition, pos);
			mousePositionDelta = AddPositions(mousePositionDelta, pos);
		}
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

std::pair<int, int> Mouse::GetPos()
{
	return mousePosition;
}

int Mouse::GetPosX()
{
	return mousePosition.first;
}

int Mouse::GetPosY()
{
	return mousePosition.second;
}

std::pair<int, int> Mouse::GetPosDelta()
{
	std::pair<int, int> d = mousePositionDelta;
	mousePositionDelta = { 0, 0 };
	return d;
}

int Mouse::GetPosDeltaX()
{
	return mousePositionDelta.first;
}

int Mouse::GetPosDeltaY()
{
	return mousePositionDelta.second;
}

int Mouse::GetWheelDelta()
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