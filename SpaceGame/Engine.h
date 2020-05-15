#pragma once

#include "Window.h"

class Engine
{
public:
	Engine();
	~Engine();
private:
	void Draw(float dt);
private:
	Window window;
	bool running = true;
};