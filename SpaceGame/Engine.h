#pragma once

#include "Window.h"

class Engine
{
public:
	Engine();
private:
	void Draw(float dt);
private:
	Window window;
	bool running = true;
};