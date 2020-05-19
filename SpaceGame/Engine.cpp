#include "Engine.h"
#include <chrono>
#include <thread>
#include <condition_variable>

Engine::Engine()
{
	controlThread = std::thread(&Engine::ControlLoop, this);

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0) || true)
	{
		if (msg.message == WM_QUIT)
		{
			std::lock_guard<std::mutex> lockGuard(mutex);
			running = false;
			break;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	std::exit((int)msg.wParam);
}

void Engine::ControlLoop()
{
	auto lastTick = std::chrono::steady_clock::now();
	auto lastRender = std::chrono::steady_clock::now();
	int ticks = 0;
	int frames = 0;//currently unused
	while (true)
	{
		{
			std::lock_guard<std::mutex> lockGuard(mutex);
			if (!running)
			{
				break;
			}
		}
		auto now = std::chrono::steady_clock::now();
		float dtTick = std::chrono::duration<float, std::milli>(now - lastTick).count();
		if (dtTick > tickTime)
		{
			Tick(ticks++, dtTick);
			lastTick = now;
			dtTick = 0;
		}

		float dtRender = std::chrono::duration<float, std::milli>(now - lastRender).count();
		if (dtRender >= maxFrameTime)
		{
			frames++;
			threadPool->QueueJob([this, ticks, dtTick, dtRender]() {Render(ticks + (dtTick / tickTime), dtRender);});
			lastRender = now;
		}
	}
}
void Engine::Tick(int tick, float dt)
{
	/*
	OutputDebugString("TICKING: ");
	OutputDebugString(std::to_string(tick).c_str());
	OutputDebugString(", tps: ");
	OutputDebugString(std::to_string(1000.0f / dt).c_str());
	OutputDebugString("\n");
	*/
	std::unique_lock<std::mutex> uniqueLock(mutex);
	//todo tick here
}

void Engine::Render(float tick, float dt)
{
	{
		std::unique_lock<std::mutex> uniqueLock(mutex);
		//todo get all variables and stuff needed for draw
	}
	/*
	OutputDebugString("DRAWING: ");
	OutputDebugString(std::to_string(tick).c_str());
	//OutputDebugString(", animate:");
	//OutputDebugString(std::to_string(time / 60).c_str());
	OutputDebugString(",");
	OutputDebugString(std::to_string(1000 / dt).c_str());
	OutputDebugString("\n");
	*/
	try {
		window->Graphics().Clear(0.0f, 1.0f, 0.0f);
		window->Graphics().drawTriangle(tick / 60, //60 is a random constant i think
			0, 
			0);
		window->Graphics().drawTriangle(tick / 60,
			window->mouse.GetPosX() / 640.0f - 1.0f,
			-window->mouse.GetPosY() / 360.0f + 1.0f);
		window->Graphics().Present(1, 0);//this can wait for vsync, for some reason flops between taking 18ms and 11ms to complete every other frame
	}
	catch (Graphics::InfoException & e)
	{
		window->SetTitle(e.what());
		OutputDebugString("Nonfatal Engine Error:");
		OutputDebugString(e.what());
	}
	catch (EngineException & e)
	{
		window->SetTitle(e.what());
		OutputDebugString("Fatal Engine Error: ");
		OutputDebugString(e.what());
		PostQuitMessage(-1);
	}
}