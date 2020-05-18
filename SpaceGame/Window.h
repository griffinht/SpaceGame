#pragma once

#include <Windows.h>
#include "Graphics.h"
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
	class Mouse
	{
	public:
		class Event
		{
		public:
			enum class Type
			{
				Move,
				LButtonDown,
				LButtonUp,
				MButtonDown,
				MButtonUp,
				MouseWheel,
				RButtonDown,
				RButtonUp
			};
		public:
			Event(Type type, WPARAM wParam, LPARAM lParam);
			Type getType();
			POINTS getPoints();
			WPARAM getWParam();
		private:
			Type type;
			POINTS points;
			WPARAM wParam;
		};
	public:
		Mouse() = default;
		std::optional<Window::Mouse::Event> GetEvent();
		void OnEvent(Window::Mouse::Event::Type type, WPARAM wParam, LPARAM lParam);
	private:
		std::queue<Window::Mouse::Event> events;
	};
	class Keyboard
	{
	public:
		class Event
		{
		public:
			enum class Type
			{
				Keydown,
				Keyup,
				Char,
			};
		public:
			Event(Type type, WPARAM wParam, LPARAM lParam);
			Type getType();
			POINTS getPoints();
			WPARAM getWParam();
		private:
			Type type;
			POINTS points;
			WPARAM wParam;
		};
	public:
		Keyboard() = default;
		std::optional<Window::Keyboard::Event> GetEvent();
		void OnEvent(Window::Keyboard::Event::Type type, WPARAM wParam, LPARAM lParam);
	private:
		std::queue<Window::Keyboard::Event> events;
	};

private:
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	const LPCSTR CLASS_NAME = "SpaceGame Class Thing";
	HINSTANCE hInstance;
	HWND hWnd;
	std::unique_ptr<::Graphics> pGraphics;
	Window::Mouse mouse;
	Window::Keyboard keyboard;
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