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
			if (true)//ticks
			{
				Draw(0);//elapsed
			}
		}
	}
}

Engine::~Engine()
{
	//todo
}

void Engine::Draw(float dt)
{
	window.Graphics().ClearBuffer(0.0f, 1.0f, 0.0f);
	window.Graphics().FlipBuffer();
}
