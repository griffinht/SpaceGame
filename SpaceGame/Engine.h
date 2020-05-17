#pragma once

#include "Window.h"
#include <mutex>
#include <condition_variable>

class Engine
{
public:
	Engine();
	~Engine();
private:
	void ControlLoop();
	void Tick(int tick, float dt);
	void Render(float tick, float dt);
private:
	std::mutex mutex;
	bool rendering = false;
	std::thread controlThread;
private:
	Window window;
	bool running = true;
private:
	const float maxFrameRate = 666;
	const float maxFrameTime = 1000.0f / maxFrameRate;
	const float tickRate = 60;
	const float tickTime = 1000.0f / tickRate; // how long each tick takes in miliseconds
	const bool vSync = true;
};