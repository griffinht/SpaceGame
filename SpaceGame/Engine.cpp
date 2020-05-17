#include "Engine.h"
#include <chrono>
#include <thread>

Engine::Engine()
	:
	window("SpaceGame"),
	updateThread(&Engine::UpdateLoop, this),
	renderThread(&Engine::RenderLoop, this)
{

}

Engine::~Engine()
{
	updateThread.join();
	renderThread.join();
}

void Engine::UpdateLoop()
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
				tick++;
				//do tick here, don't forget to lock render thread i think
			}
			//Sleep(0);
		}
	}
}

void Engine::RenderLoop()
{
	auto last = std::chrono::steady_clock::now();
	while (running)
	{
		auto now = std::chrono::steady_clock::now();
		double dt = std::chrono::duration<double, std::milli>(now - last).count();//delta time since last frame
		double time = tick + ((double)dt / tickTime);//interpolated tick
		
		if (1000 / dt <= maxFrameRate)
		{
			OutputDebugString("fps:");
			OutputDebugString(std::to_string(1000 / dt).c_str());
			OutputDebugString("\r");
			try {
				window.Graphics().ClearBuffer(0.0f, 1.0f, 0.0f);
				window.Graphics().drawTriangle((time) / 60);//60 is a random constant i think
				window.Graphics().FlipBuffer();
			}
			catch (Graphics::InfoException & e)
			{
				OutputDebugString("Nonfatal Engine Error:");
				OutputDebugString(e.what());
			}
			catch (EngineException & e)
			{
				OutputDebugString("Fatal Engine Error: ");
				OutputDebugString(e.what());
				PostQuitMessage(-1);
			}
			last = now;
			frame++;//unused
		}
	}
}
