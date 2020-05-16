#include "Engine.h"
#include <chrono>

Engine::Engine() : window("SpaceGame")
{
	auto last = std::chrono::steady_clock::now();
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
			auto now = std::chrono::steady_clock::now();
			double dt = std::chrono::duration<double, std::milli>(now - last).count();//todo double or float?
			if (dt > tickTime)
			{
				last = now;
				dt = 0;
				tick++;
				//do tick here
			}
			Draw(tick, dt);//check if exceeds max framerate
			//Sleep(0);
		}
	}
}

void Engine::Draw(int tick, double dt)
{
	double time = tick + ((double)dt / tickTime);
	try {
		window.Graphics().ClearBuffer(0.0f, 1.0f, 0.0f);
		window.Graphics().drawTriangle((time)/60);//todo change 60 to tickrate
		window.Graphics().FlipBuffer();
	}
	catch (Graphics::InfoException& e)
	{
		OutputDebugString("Nonfatal Engine Error:");
		OutputDebugString(e.what());
	}
	catch (EngineException& e)
	{
		OutputDebugString("Fatal Engine Error: ");
		OutputDebugString(e.what());
		PostQuitMessage(-1);
	}
}
