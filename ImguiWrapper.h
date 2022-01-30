#pragma once
#include "imGui/imgui.h"
#include "imGui/imgui_impl_win32.h"
#include "imGui/imgui_impl_dx12.h"
#include <d3d12.h>
#include <dxgi1_4.h>
#include <tchar.h>
#include <Windows.h>
#include <wrl.h>

class ImguiWrapper {
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	ImguiWrapper(HWND hwnd, class Dx12Wrapper* dx12, class Input* input, class Game* game);
	~ImguiWrapper();
	void Draw();
private:
	class Dx12Wrapper* mDx12Wrapper;
	class Input* mInput;
	class Game* mGame;
	ComPtr<ID3D12DescriptorHeap>mDescHeap;
	ImVec4 mClearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};