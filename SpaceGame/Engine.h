#pragma once

#include "Window.h"
#include <mutex>

class Engine
{
public:
	Engine();
	~Engine();
public:
	void UpdateLoop();
	void RenderLoop();
private:
	std::mutex mutex;
	std::thread updateThread;
	std::thread renderThread;
private:
	Window window;
	bool running = true;
	int ticks = 0;
	int frames = 0;
private:
	const float maxFrameRate = 666;
	const float tickRate = 60;
	const double tickTime = 1000.0 / tickRate; // how long each tick takes in miliseconds
	const bool vSync = true;
};