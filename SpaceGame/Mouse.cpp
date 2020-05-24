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
	if (!rawInput)
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
		case Event::Type::Move:
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
			break;
		case Event::Type::MouseWheel:
			wheelDelta += GET_WHEEL_DELTA_WPARAM(wParam);
			break;
		}

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

		if (rawMouse.ulButtons & RI_MOUSE_LEFT_BUTTON_DOWN)
			lButtonPressed = true;
		if (rawMouse.ulButtons & RI_MOUSE_LEFT_BUTTON_UP)
			lButtonPressed = false;
		if (rawMouse.ulButtons & RI_MOUSE_MIDDLE_BUTTON_DOWN)
			mButtonPressed = true;
		if (rawMouse.ulButtons & RI_MOUSE_MIDDLE_BUTTON_UP)
			mButtonPressed = false;
		if (rawMouse.ulButtons & RI_MOUSE_RIGHT_BUTTON_DOWN)
		{
			SetConstrained(true);
			rButtonPressed = true;
		}
		if (rawMouse.ulButtons & RI_MOUSE_RIGHT_BUTTON_UP)
		{ 
			SetConstrained(false);
			rButtonPressed = true; 
		}
		if (rawMouse.ulButtons & RI_MOUSE_BUTTON_4_DOWN)
		{

		}
		if (rawMouse.ulButtons & RI_MOUSE_BUTTON_4_UP)
		{

		}
		if (rawMouse.ulButtons & RI_MOUSE_BUTTON_5_DOWN)
		{

		}
		if (rawMouse.ulButtons & RI_MOUSE_BUTTON_5_UP)
		{

		}

		if (rawMouse.usButtonFlags & RI_MOUSE_WHEEL)
		{
			wheelDelta += rawMouse.usButtonData;
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