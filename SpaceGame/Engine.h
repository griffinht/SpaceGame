#pragma once

#include "Window.h"

class Engine
{
public:
	Engine();
private:
	void Draw(int tick, double dt);
private:
	Window window;
	bool running = true;
	int tick = 0;
	float tickRate = 60;
	double tickTime = 1000.0 / tickRate; // how long each tick takes in miliseconds
};