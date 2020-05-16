#pragma once

#define WINDOW_LAST_EXCEPTION() Window::HrException(__LINE__, __FILE__, GetLastError())
#define WINDOW_NOGRAPHICS_EXCEPTION() Window::NoGraphicsException(__LINE__, __FILE__)