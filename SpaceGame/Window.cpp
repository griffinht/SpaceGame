#include "Window.h"
#include <sstream>
#include "WindowThrowMacros.h"

Window::Window(const char* name)
{
	hInstance = GetModuleHandle(nullptr);

	// window class
	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = &Window::StaticWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = sizeof(Window*);
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hIcon = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = CLASS_NAME;
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);

	//window instance
	hWnd = CreateWindowEx(
		0, CLASS_NAME,
		name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
		nullptr, nullptr, hInstance, static_cast<LPVOID>(this)
	);

	if (hWnd == nullptr)
	{
		throw WINDOW_LAST_EXCEPTION();
	}

	ShowWindow(hWnd, SW_SHOW);

	pGraphics = std::make_unique<::Graphics>(hWnd);
}

Window::~Window()
{
	UnregisterClass(CLASS_NAME, hInstance);

	DestroyWindow(hWnd);
}

Graphics& Window::Graphics()
{
	if (!pGraphics)
	{
		throw WINDOW_NOGRAPHICS_EXCEPTION();
	}
	return *pGraphics;
}

void Window::SetTitle(const char* name)
{
	SetWindowText(hWnd, name);
}

LRESULT Window::StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		CREATESTRUCT* pCS = reinterpret_cast<CREATESTRUCT*>(lParam);
		LPVOID pThis = pCS->lpCreateParams;
		SetWindowLongPtrW(hWnd, 0, reinterpret_cast<LONG_PTR>(pThis));
	}

	Window* pWnd = reinterpret_cast<Window*>(GetWindowLongPtrW(hWnd, 0));
	return pWnd->WndProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	std::stringstream ss;

	ss << msg;
	//OutputDebugString(ss.str().c_str());
	//OutputDebugString("\n");

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		keyboard.OnEvent(Window::Keyboard::Event::Type::Keydown, wParam, lParam);
		break;
	case WM_KEYUP:
		keyboard.OnEvent(Window::Keyboard::Event::Type::Keyup, wParam, lParam);
		break;
	case WM_CHAR:
		keyboard.OnEvent(Window::Keyboard::Event::Type::Char, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		mouse.OnEvent(Window::Mouse::Event::Type::Move, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		mouse.OnEvent(Window::Mouse::Event::Type::LButtonDown, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		mouse.OnEvent(Window::Mouse::Event::Type::LButtonUp, wParam, lParam);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr)
{
	char* pMessageBuffer = nullptr;
	const DWORD nMessageLength = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMessageBuffer), 0, nullptr
	);

	if (nMessageLength == 0)
	{
		return "Unknown Error";
	}

	std::string error = pMessageBuffer;
	LocalFree(pMessageBuffer);
	return error;
}

Window::HrException::HrException(int line, const char* file, HRESULT hr)
	:
	Exception(line, file),
	hr(hr)
{}

const char* Window::HrException::what() const
{
	std::ostringstream oss;
	oss << GetType() << " generated error code 0x" << std::hex << std::uppercase << GetErrorCode()
		<< " in " << GetOriginString() << std::endl;
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::HrException::GetType() const
{
	return "Window Exception";
}

HRESULT Window::HrException::GetErrorCode() const
{
	return hr;
}

std::string Window::HrException::GetErrorDescription() const
{
	return Exception::TranslateErrorCode(hr);
}


const char* Window::NoGraphicsException::GetType() const
{
	return "Window Exception [No Graphics]";
}

Window::Mouse::Event::Event(Type type, WPARAM wParam, LPARAM lParam)
{
	this->type = type;
	this->wParam = wParam;
	this->points = MAKEPOINTS(lParam);
}

Window::Mouse::Event::Type Window::Mouse::Event::getType()
{
	return type;
}

POINTS Window::Mouse::Event::getPoints()
{
	return points;
}

WPARAM Window::Mouse::Event::getWParam()
{
	return wParam;
}

std::optional<Window::Mouse::Event> Window::Mouse::GetEvent()
{
	if (events.empty())
	{
		return std::optional<Window::Mouse::Event>();
	}
	else
	{
		return std::optional<Window::Mouse::Event>(events.front());
		events.pop();
	}
}

void Window::Mouse::OnEvent(Window::Mouse::Event::Type type, WPARAM wParam, LPARAM lParam)
{
	events.push(Window::Mouse::Event::Event(type, wParam, lParam));
}

Window::Keyboard::Event::Event(Type type, WPARAM wParam, LPARAM lParam)
{
	this->type = type;
	this->wParam = wParam;
	this->points = MAKEPOINTS(lParam);
}

Window::Keyboard::Event::Type Window::Keyboard::Event::getType()
{
	return type;
}

POINTS Window::Keyboard::Event::getPoints()
{
	return points;
}

WPARAM Window::Keyboard::Event::getWParam()
{
	return wParam;
}

std::optional<Window::Keyboard::Event> Window::Keyboard::GetEvent()
{
	if (events.empty())
	{
		return std::optional<Window::Keyboard::Event>();
	}
	else
	{
		return std::optional<Window::Keyboard::Event>(events.front());
		events.pop();
	}
}

void Window::Keyboard::OnEvent(Window::Keyboard::Event::Type type, WPARAM wParam, LPARAM lParam)
{
	events.push(Window::Keyboard::Event::Event(type, wParam, lParam));
}
