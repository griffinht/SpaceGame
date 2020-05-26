#include "Mouse.h"
#include <string>
#include <sstream>

Mouse::Event::Event(Type type, std::pair<float, float> position)
{
	this->type = type;
	this->position = position;
}

Mouse::Event::Type Mouse::Event::GetType()
{
	return type;
}

std::pair<float, float> Mouse::Event::GetPos()
{
	return position;
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
	switch (type)
	{
	case Event::Type::LButtonDown:
		lButtonPressed = false;
		events.push(Event::Event(type, position));
		break;
	case Event::Type::LButtonUp:
		lButtonPressed = true;
		events.push(Event::Event(type, position));
		break;
	case Event::Type::MButtonDown:
		mButtonPressed = true;
		events.push(Event::Event(type, position));
		break;
	case Event::Type::MButtonUp:
		mButtonPressed = false;
		events.push(Event::Event(type, position));
		break;
	case Event::Type::RButtonDown:
		SetConstrained(true);
		rButtonPressed = true;
		events.push(Event::Event(type, position));
		break;
	case Event::Type::RButtonUp:
		SetConstrained(false);
		rButtonPressed = false;
		events.push(Event::Event(type, position));
		break;
	case Event::Type::XButton1Down:
		xButton1Pressed = true;
		events.push(Event::Event(type, position));
		break;
	case Event::Type::XButton1Up:
		xButton1Pressed = false;
		events.push(Event::Event(type, position));
		break;
	case Event::Type::XButton2Down:
		xButton2Pressed = true;
		events.push(Event::Event(type, position));
		break;
	case Event::Type::XButton2Up:
		xButton2Pressed = false;
		break;
	case Event::Type::MouseWheel:
		wheelDelta += GET_WHEEL_DELTA_WPARAM(wParam);
		break;
	}

	while (maxBufferSize > 0 && events.size() > maxBufferSize)
	{
		events.pop();
	}

	if (!rawInput && type == Event::Type::Move)
	{
		if (constrained)
		{
			POINT pt;
			GetCursorPos(&pt);
			positionDelta = { positionDelta.first + (short)pt.x - center.x, positionDelta.second + (short)pt.y - center.y };
			SetCursorPos(center.x, center.y);
		}
		else
		{
			POINTS pt = MAKEPOINTS(lParam);
			positionDelta = { positionDelta.first + pt.x - position.first, positionDelta.second + pt.y - position.second };
			position = { pt.x, pt.y };
		}
	}
}

void Mouse::OnRawEvent(tagRAWMOUSE rawMouse)
{
	if (rawInput)
	{
		if (rawMouse.usFlags & MOUSE_MOVE_ABSOLUTE)
		{
			bool isVirtualDesktop = (rawMouse.usFlags & MOUSE_VIRTUAL_DESKTOP) == MOUSE_VIRTUAL_DESKTOP;
			float xChange = (rawMouse.lLastX / 65535.0f) * GetSystemMetrics(isVirtualDesktop ? SM_CXVIRTUALSCREEN : SM_CXSCREEN);
			float yChange = (rawMouse.lLastY / 65535.0f) * GetSystemMetrics(isVirtualDesktop ? SM_CYVIRTUALSCREEN : SM_CYSCREEN);

			positionDelta = { positionDelta.first + rawMouse.lLastX - position.first, positionDelta.second + rawMouse.lLastY - position.second };

			position.first += xChange;
			position.second += yChange;
		}
		else
		{
			positionDelta = { positionDelta.first + rawMouse.lLastX, positionDelta.second + rawMouse.lLastY };

			position.first += rawMouse.lLastX;
			position.second += rawMouse.lLastY;
		}
	}
}

std::pair<float, float> Mouse::GetPos()
{
	return position;
}

std::pair<float, float> Mouse::GetPosDelta()
{
	std::pair<float, float> d = positionDelta;
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

bool Mouse::XButton1Pressed()
{
	return xButton1Pressed;
}

bool Mouse::XButton2Pressed()
{
	return xButton2Pressed;
}

void Mouse::SetRawInput(bool rawInput)
{
	if (rawInput && !this->rawInput)
	{
		RAWINPUTDEVICE rid[1];
		rid[0].usUsagePage = 0x01;
		rid[0].usUsage = 0x02; // mouse
		rid[0].dwFlags = 0;
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
	SetCursorVisibility(!constrain);
	if (!constrain)
	{
		SetCursorPos(center.x, center.y);
	}
	if (constrained)
	{
		const RECT c = { 0, 0, size.x, size.y };
		ClipCursor(&c);
	}
	else
	{
		ClipCursor(nullptr);
	}
}

void Mouse::SetSize(POINTS c)
{
	size = c;
	center = { (short)(c.x / 2), (short)(c.y / 2) };
}

void Mouse::SetCursorVisibility(bool show)
{
	if (show)
	{
		while (ShowCursor(show));
	}
	else
	{
		while (!ShowCursor(show));
	}
}
