#pragma once
#include "Window.h"
#include "Windows.h"
#ifdef _DEBUG
#include <iostream>
#endif

class Window {
public:
	Window(int w = 1280, int h = 720);
	~Window();
	void Init();
	void Run();
private:
	WNDCLASSEX mWindowClass;
	HWND mHwnd;
	const unsigned int mWidth;
	const unsigned int mHeight;
};