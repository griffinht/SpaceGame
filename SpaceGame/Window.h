#pragma once

#include <Windows.h>
#include "Graphics.h"
#include <string>
#include <memory>
#include <optional>

class Window
{
public:
	Window(const char* name);
	~Window();
	Graphics& Graphics();
private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	const LPCSTR CLASS_NAME = "SpaceGame Class Thing";
	HINSTANCE hInstance;
	HWND hWnd;
	std::unique_ptr<::Graphics> pGraphics;
};