#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class Dx12Wrapper {
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	Dx12Wrapper();
	HRESULT InitDXGIDevice();
	HRESULT InitCommand();
	HRESULT InitSwapChain(const HWND& hwnd, const unsigned int w, const unsigned int h);
	HRESULT InitRenderTargets();

	void StartDraw();
	void EndDraw();
private:
	// DXGI関連
	ComPtr<IDXGIFactory4> mDxgiFactory = nullptr;
	ComPtr<IDXGISwapChain4> mSwapChain = nullptr;

	// DirectX12関連
	ComPtr<ID3D12Device> mDev = nullptr;													// デバイス
	ComPtr<ID3D12CommandAllocator> mCmdAllocator = nullptr;				// コマンドアロケータ
	ComPtr<ID3D12GraphicsCommandList> mCmdList = nullptr;					// コマンドリスト
	ComPtr<ID3D12CommandQueue> mCmdQueue = nullptr;								// コマンドキュー
	ComPtr<ID3D12DescriptorHeap> mRtvHeaps = nullptr;							// ディスクリプタヒープ
	std::vector<ID3D12Resource*> mBackBuffers;										// バックバッファ
};