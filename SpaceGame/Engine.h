#pragma once

#include "Window.h"
#include "ThreadPool.h"
#include "Box.h"
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "Player.h"

class Engine
{
public:
	Engine();
private:
	void ControlLoop();
	void Tick(int tick, float dt);
	void Render(float tick, float dt);
	void Resize(int width, int height);
private:
	std::unique_ptr<Window> window = std::make_unique<Window>("SpaceGame", Window::WINDOWED);
	std::unique_ptr<ThreadPool> threadPool = std::make_unique<ThreadPool>(1, 1);
	std::thread controlThread;
	std::atomic<bool> running = true;
private:
	const float maxFrameRate = 666;
	const float maxFrameTime = 1000.0f / maxFrameRate;
	const float tickRate = 60;
	const float tickTime = 1000.0f / tickRate; // how long each tick takes in miliseconds
	const bool vSync = true;
	Player player;
	std::unique_ptr<Box> box;
private:
	std::vector<std::unique_ptr<Drawable>> drawables;
};