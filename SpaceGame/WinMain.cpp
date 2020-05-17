#include "Engine.h"
#include <sstream>
#include <thread>

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	Engine();
	return -1;
}