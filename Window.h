#pragma once
#include "Window.h"
#include "Windows.h"


class Window {
public:
	Window(int w = 1280, int h = 720);
	~Window();
	void Init();
	void Run();
	HWND GetHwnd();
	const unsigned int GetWidth() { return mWidth; }
	const unsigned int GetHeight() { return mHeight; }
private:
	WNDCLASSEX mWindowClass;
	HWND mHwnd;
	const unsigned int mWidth;
	const unsigned int mHeight;
};