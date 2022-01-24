#include "Window.h"
#include "imGui/imgui.h"
#include "imGui/imgui_impl_win32.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

Window::Window(int w, int h) :mWidth(w), mHeight(h) {

}

Window::~Window() {
	UnregisterClass(mWindowClass.lpszClassName, mWindowClass.hInstance);
}

void Window::Init() {
	// ウィンドウクラスの登録
	mWindowClass = {};
	mWindowClass.cbSize = sizeof(WNDCLASSEX);
	mWindowClass.lpfnWndProc = (WNDPROC)WindowProcedure;
	mWindowClass.lpszClassName = "DX12Sample";
	mWindowClass.hInstance = GetModuleHandle(nullptr);
	RegisterClassEx(&mWindowClass);
	RECT wrc = { 0, 0, mWidth, mHeight };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの作成
	mHwnd = CreateWindow(mWindowClass.lpszClassName, "DX12テスト", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, wrc.right - wrc.left, wrc.bottom - wrc.top, nullptr, nullptr, mWindowClass.hInstance, nullptr);
}

void Window::Run() {
	ShowWindow(mHwnd, SW_SHOW);
}

HWND Window::GetHwnd() {
	return this->mHwnd;
}

