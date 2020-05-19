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

	RECT rect = RECT();
	rect.left = 0;
	rect.right = 1280;
	rect.top = 0;
	rect.bottom = 720;

	DWORD dwExStyle = 0;
	DWORD dwStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_SIZEBOX;
	AdjustWindowRectEx(&rect, dwStyle, false, dwExStyle);
	//window instance
	hWnd = CreateWindowEx(
		dwExStyle,
		CLASS_NAME,
		name,
		dwStyle,
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		rect.right - rect.left, 
		rect.bottom - rect.top,
		nullptr, 
		nullptr, 
		hInstance, 
		static_cast<LPVOID>(this)
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

	delete& mouse;
	delete& keyboard;
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
		keyboard.OnEvent(Keyboard::Event::Type::Keydown, wParam, lParam);
		break;
	case WM_KEYUP:
		keyboard.OnEvent(Keyboard::Event::Type::Keyup, wParam, lParam);
		break;
	case WM_CHAR:
		keyboard.OnEvent(Keyboard::Event::Type::Char, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		mouse.OnEvent(Mouse::Event::Type::Move, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		mouse.OnEvent(Mouse::Event::Type::LButtonDown, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		mouse.OnEvent(Mouse::Event::Type::LButtonUp, wParam, lParam);
		break;
	case WM_SIZE:
		if (pGraphics)
		{
			Graphics().ResizeBuffers(LOWORD(lParam), HIWORD(lParam));
		}
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