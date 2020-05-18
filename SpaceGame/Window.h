#pragma once

#include <Windows.h>
#include "Graphics.h"
#include "Mouse.h"
#include "Keyboard.h"
#include <string>
#include <memory>
#include <optional>
#include <queue>

class Window
{
public:
	Window(const char* name);
	~Window();
	Graphics& Graphics();
	void SetTitle(const char* name);



private:
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	const LPCSTR CLASS_NAME = "SpaceGame Class Thing";
	HINSTANCE hInstance;
	HWND hWnd;
	std::unique_ptr<::Graphics> pGraphics;
	Mouse mouse;
	Keyboard keyboard;
public:
	class Exception : public EngineException
	{
		using EngineException::EngineException;
	public:
		static std::string TranslateErrorCode(HRESULT hr);
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr);
		const char* what() const override;
		const char* GetType() const override;
		HRESULT GetErrorCode() const;
		std::string GetErrorDescription() const;
	private:
		HRESULT hr;
	};
	class NoGraphicsException : public Exception
	{
	public:
		using Exception::Exception;
		const char* GetType() const override;
	};
};