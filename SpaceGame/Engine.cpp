#include "Engine.h"

Engine::Engine() : window("SpaceGame")
{
	while (running)
	{
		MSG msg;
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				std::exit((int)msg.wParam);
			}
			TranslateMessage(&msg); //optional, remove?
			DispatchMessage(&msg);
		}
		else
		{
			Sleep(0);
			//game stuff
		}
	}
}

Engine::~Engine()
{
	//todo
}

void Engine::Draw(float dt)
{
	//todo
}
