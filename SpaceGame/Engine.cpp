#include "Engine.h"
#include <chrono>
#include <thread>
#include <condition_variable>
#include <random>

Engine::Engine()
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);

	for (auto i = 0; i < 80; i++)
	{
		drawables.push_back(std::make_unique<Box>(
			window->Graphics(), rng, adist,
			ddist, odist, rdist
		));
	}
	window->Graphics().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));//wrong

	controlThread = std::thread(&Engine::ControlLoop, this);

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0) || true)
	{
		if (msg.message == WM_QUIT)
		{
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

	//todo tick here
}

void Engine::Render(float tick, float dt)
{
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
		for (auto &d : drawables)
		{
			d->Update(dt / 100.0f);

			d->Draw(window->Graphics());
		}
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