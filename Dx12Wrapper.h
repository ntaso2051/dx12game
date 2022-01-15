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
	Dx12Wrapper();
	HRESULT InitDXGIDevice();
	HRESULT InitCommand();
private:
	// DXGI�֘A
	ComPtr<IDXGIFactory4> mDxgiFactory = nullptr;
	ComPtr<IDXGISwapChain4> mSwapChain = nullptr;

	// DirectX12�֘A
	ComPtr<ID3D12Device> mDev = nullptr;//�f�o�C�X
	ComPtr<ID3D12CommandAllocator> mCmdAllocator = nullptr;//�R�}���h�A���P�[�^
	ComPtr<ID3D12GraphicsCommandList> mCmdList = nullptr;//�R�}���h���X�g
	ComPtr<ID3D12CommandQueue> mCmdQueue = nullptr;//�R�}���h�L���[
};