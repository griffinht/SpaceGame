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

Mouse::Mouse()
{
	SetRawInput(rawInput);
}

void Mouse::OnEvent(Mouse::Event::Type type, WPARAM wParam, LPARAM lParam)
{
	if (!rawInput)
	{
		switch (type)
		{
		case Event::Type::Move:
			if (constrained)
			{
				POINT pt;
				GetCursorPos(&pt);
				positionDelta = { positionDelta.x + (short)pt.x - center.x, positionDelta.y + (short)pt.y - center.y };
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
			SetConstrained(true);
			rButtonPressed = true;
			break;
		case Event::Type::RButtonUp:
			SetConstrained(false);
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
}

void Mouse::OnRawEvent(tagRAWMOUSE rawMouse)
{
	if (rawInput)
	{

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

void Mouse::SetRawInput(bool rawInput)
{
	if (rawInput && !this->rawInput)
	{
		RAWINPUTDEVICE rid[1];
		rid[0].usUsagePage = 0x01;
		rid[0].usUsage = 0x02; // mouse
		rid[0].dwFlags = RIDEV_NOLEGACY;
		rid[0].hwndTarget = 0;

		RegisterRawInputDevices(rid, sizeof(rid) / sizeof(rid[0]), sizeof(rid[0]));
	}
	else if(!rawInput && this->rawInput)
	{
		RAWINPUTDEVICE rid[1];
		rid[0].usUsagePage = 0x01;
		rid[0].usUsage = 0x02; // mouse
		rid[0].dwFlags = RIDEV_REMOVE;
		rid[0].hwndTarget = 0;
		RegisterRawInputDevices(rid, sizeof(rid) / sizeof(rid[0]), sizeof(rid[0]));
	}
	this->rawInput = rawInput;
}

void Mouse::SetConstrained(bool constrain)
{
	constrained = constrain;
	ShowCursor(!constrain);
}

void Mouse::SetCenter(POINTS c)
{
	center = c;
}