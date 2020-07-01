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
	running = false;
	controlThread.join();
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
	threadPool->~ThreadPool();
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
	OutputDebugString(std::to_string(time / 60).c_str());
	OutputDebugString(",");
	OutputDebugString(std::to_string(1000 / dt).c_str());
	OutputDebugString("\n");
	*/
	
	/*
	OutputDebugString(std::to_string(pt.x).c_str());
	OutputDebugString(",");
	OutputDebugString(std::to_string(pt.y).c_str());
	OutputDebugString(" = ");
	OutputDebugString(std::to_string((float)pt.y / (float)window->Graphics().GetBackBufferHeight()).c_str());
	OutputDebugString(",");
	OutputDebugString(std::to_string((float)pt.x / (float)window->Graphics().GetBackBufferWidth()).c_str());
	OutputDebugString("\n");
	*/
	float fov = 0.0f;
	if (window->mouse.XButton1Pressed())
		fov += 0.01f;
	if (window->mouse.XButton2Pressed())
		fov -= 0.01f;
	player.GetCamera().ChangeFOV(fov);

	std::pair<float, float> pt = window->mouse.GetPosDelta();
	DirectX::XMFLOAT2 lookRotation{ pt.second / (float)window->Graphics().GetBackBufferHeight(), pt.first / (float)window->Graphics().GetBackBufferWidth() };
	DirectX::XMFLOAT3 rotation = { 0, 0, 0 };
	if (window->keyboard.KeyPressed(0x12))// alt to look
	{
		player.ChangePlayerLookPitchYaw(lookRotation);
	}
	else
	{
		rotation.x = lookRotation.x * -1;
		rotation.y = lookRotation.y * -1;
	}

	if (window->keyboard.KeyPressed(0x45))//e
		rotation.z += dt * 0.001f;
	if (window->keyboard.KeyPressed(0x51))//q
		rotation.z -= dt * 0.001f;//todo regular units
	player.ChangePlayerRotationVelocity(DirectX::XMLoadFloat3(&rotation));

	DirectX::XMFLOAT3 translation{};

	if (window->keyboard.KeyPressed(0x44))//d
		translation.x += dt * 0.001f;
	if (window->keyboard.KeyPressed(0x41))//a
		translation.x -= dt * 0.001f;
	if (window->keyboard.KeyPressed(VK_SPACE))
		translation.y += dt * 0.001f;
	if (window->keyboard.KeyPressed(0x43))//c
		translation.y -= dt * 0.001f;
	if (window->keyboard.KeyPressed(0x57))//w
		translation.z += dt * 0.001f;
	if (window->keyboard.KeyPressed(0x53))//s
		translation.z -= dt * 0.001f;

	player.ChangePlayerMovementVelocity(DirectX::XMLoadFloat3(&translation));

	try {
		player.Update(dt);
		window->Graphics().SetProjection(player.GetProjection());
		window->Graphics().Clear(0.1f, 0.1f, 0.1f);
		for (auto &d : drawables)
		{
			d->Update(dt / 1000.0f);

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

void Engine::Resize(int width, int height)
{
	player.GetCamera().SetAspectRatio((float)width / (float)height);
}
