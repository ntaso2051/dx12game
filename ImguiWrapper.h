#pragma once
#include "imGui/imgui.h"
#include "imGui/imgui_impl_win32.h"
#include "imGui/imgui_impl_dx12.h"
#include <d3d12.h>
#include <dxgi1_4.h>
#include <tchar.h>
#include <Windows.h>
#include <wrl.h>
#include <vector>
#include <string>

class ImguiWrapper {
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	ImguiWrapper(HWND hwnd, class Dx12Wrapper* dx12, class Input* input, class Game* game);
	~ImguiWrapper();
	void Draw();
	void Cout(std::string log);
	void ClearLogs();
private:
	class Dx12Wrapper* mDx12Wrapper;
	class Input* mInput;
	class Game* mGame;
	ComPtr<ID3D12DescriptorHeap>mDescHeap;
	class Item* mSelectedItem;
	bool mItemCmd;
	std::vector<std::string> mLogs;
};