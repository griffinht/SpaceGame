#include "Engine.h"
#include <chrono>
#include <thread>

Engine::Engine()
	:
	window("SpaceGame")
{
	renderThread = std::thread(&Engine::RenderLoop, this);
	updateThread = std::thread(&Engine::UpdateLoop, this);
	
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
	lastUpdate = std::chrono::steady_clock::now();
	while (running)
	{
		auto now = std::chrono::steady_clock::now();
		double dt = std::chrono::duration<double, std::milli>(now - lastUpdate).count();//todo double or float? also does this need to be locked
		if (dt > tickTime)
		{
			std::unique_lock<std::mutex> unique(mutex);
			lastUpdate = now;
			ticks++;
			OutputDebugString("TICKING\n");
			unique.unlock();
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
		
		if (1000 / dt <= maxFrameRate && std::chrono::duration<double, std::milli>(now - lastUpdate).count() < tickTime) //check if exceeds max framerate and check if a tick is about to happen, in that case don't go
		{//draw right before a tick causes synchronization and stuttering problems when the update rate and refresh rate match
			std::unique_lock<std::mutex> unique(mutex);//consider not locking if we can get everything (variables and stuff) we need then do the heavy duty gpu functions
			last = now;
			frames++;//unused
			//todo get all variables and stuff needed for draw
			unique.unlock();
			OutputDebugString("DRAWING:tick:");
			OutputDebugString(std::to_string(ticks).c_str());
			OutputDebugString(", animate:");
			OutputDebugString(std::to_string(time / 60).c_str());
			OutputDebugString("fps:");
			OutputDebugString(std::to_string(1000 / dt).c_str());
			OutputDebugString("\n");
			try {
				window.Graphics().ClearBuffer(0.0f, 1.0f, 0.0f);
				window.Graphics().drawTriangle(time / 60);//60 is a random constant i think
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
		//Sleep(0);
	}
}
