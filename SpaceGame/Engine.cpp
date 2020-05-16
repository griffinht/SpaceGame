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

void Engine::Draw(float dt)
{
	try {
		window.Graphics().ClearBuffer(0.0f, 1.0f, 0.0f);
		window.Graphics().drawTriangle();
		window.Graphics().FlipBuffer();
	}
	catch (EngineException& e)
	{
		OutputDebugString("Fatal Exception, terminating\n");
		OutputDebugString(e.what());
		PostQuitMessage(-1);
	}
}
