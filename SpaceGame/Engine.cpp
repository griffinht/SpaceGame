#include "Engine.h"
#include <chrono>
#include <thread>

Engine::Engine()
	:
	window("SpaceGame")
{
	updateThread = std::thread(&Engine::UpdateLoop, this);
	renderThread = std::thread(&Engine::RenderLoop, this);
	MSG msg;
	bool result;
	while ((result = GetMessage(&msg, nullptr, 0, 0)) != 0)
	{
		if (result == -1)
		{
			OutputDebugString("Error -1 in windows message");
			std::exit((int)msg.wParam);
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	running = false;
	updateThread.join();
	renderThread.join();
	std::exit(msg.wParam);
}

Engine::~Engine()
{
	running = false;
	updateThread.join();
	renderThread.join();
}

void Engine::UpdateLoop()
{
	auto last = std::chrono::steady_clock::now();
	while (running)
	{
		auto now = std::chrono::steady_clock::now();
		double dt = std::chrono::duration<double, std::milli>(now - last).count();//todo double or float?
		if (dt > tickTime)
		{
			std::unique_lock<std::mutex> unique(mutex);
			last = now;
			ticks++;
			OutputDebugString("ticking\n");
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
		double time = ticks + ((double)dt / tickTime);//interpolated tick
		
		if (1000 / dt <= maxFrameRate)
		{
			std::unique_lock<std::mutex> unique(mutex);//consider not locking if we can get everything (variables and stuff) we need then do the heavy duty gpu functions
			last = now;
			frames++;//unused
			OutputDebugString("tick:");
			OutputDebugString(std::to_string(ticks).c_str());
			OutputDebugString(", animate:");
			OutputDebugString(std::to_string(time / 60).c_str());
			OutputDebugString("fps:");
			OutputDebugString(std::to_string(1000 / dt).c_str());
			OutputDebugString("\n");
			try {
				window.Graphics().ClearBuffer(0.0f, 1.0f, 0.0f);
				window.Graphics().drawTriangle(time / 60);//60 is a random constant i think
				unique.unlock();//must unlock before waiting to flip the buffer
				window.Graphics().FlipBuffer();
			}
			catch (Graphics::InfoException & e)
			{
				window.SetTitle(e.what());
				OutputDebugString("Nonfatal Engine Error:");
				OutputDebugString(e.what());
			}
			catch (EngineException & e)
			{
				window.SetTitle(e.what());
				OutputDebugString("Fatal Engine Error: ");
				OutputDebugString(e.what());
				PostQuitMessage(-1);
			}
		}
		Sleep(0);
	}
}
