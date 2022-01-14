#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class Dx12Wrapper {
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
private:
	// DXGIŠÖ˜A
	ComPtr<IDXGIFactory4> mDxgiFactory = nullptr;
	ComPtr<IDXGISwapChain4> mSwapChain = nullptr;

	// DirectX12ŠÖ˜A

};