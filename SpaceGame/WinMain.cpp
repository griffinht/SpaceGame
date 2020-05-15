#include "Window.h"
#include <sstream>

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	Window* window = new Window("SpaceGame");

	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0) > 0))
	{
		TranslateMessage(&msg); //optional, remove?
		DispatchMessage(&msg);
	}
	
	delete window;

	if (gResult == -1)
	{	
		return -1;
	}
	else
	{
		return msg.wParam;
	}
}