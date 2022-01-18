#include "Dx12Wrapper.h"
#include <vector>
#include <string>
#include <cassert>
#include <DirectXMath.h>

using namespace DirectX;

Dx12Wrapper::Dx12Wrapper() {

}

void Dx12Wrapper::EnableDebugLayer() {
	auto result = D3D12GetDebugInterface(IID_PPV_ARGS(&mDebugLayer));
	mDebugLayer->EnableDebugLayer();
}

HRESULT Dx12Wrapper::InitDXGIDevice() {
#ifdef _DEBUG
	auto result = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(mDxgiFactory.ReleaseAndGetAddressOf()));
#else
	auto result = CreateDXGIFactory1(IID_PPV_ARGS(mDxgiFactory.ReleaseAndGetAddressOf()));
#endif
	//DirectX12まわり初期化
	//フィーチャレベル列挙
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	if (FAILED(result)) {
		return result;
	}
	std::vector <IDXGIAdapter*> adapters;
	IDXGIAdapter* tmpAdapter = nullptr;
	for (int i = 0; mDxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; ++i) {
		adapters.push_back(tmpAdapter);
	}
	for (auto adpt : adapters) {
		DXGI_ADAPTER_DESC adesc = {};
		adpt->GetDesc(&adesc);
		std::wstring strDesc = adesc.Description;
		if (strDesc.find(L"NVIDIA") != std::string::npos) {
			tmpAdapter = adpt;
			break;
		}
	}
	result = S_FALSE;
	//Direct3Dデバイスの初期化
	D3D_FEATURE_LEVEL featureLevel;
	for (auto l : levels) {
		if (SUCCEEDED(D3D12CreateDevice(tmpAdapter, l, IID_PPV_ARGS(mDev.ReleaseAndGetAddressOf())))) {
			featureLevel = l;
			result = S_OK;
			break;
		}
	}
	return result;
}

HRESULT Dx12Wrapper::InitCommand() {
	auto result = mDev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(mCmdAllocator.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		assert(0);
		return result;
	}
	result = mDev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCmdAllocator.Get(), nullptr, IID_PPV_ARGS(mCmdList.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		assert(0);
		return result;
	}
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	result = mDev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(mCmdQueue.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));
	return result;
}

HRESULT Dx12Wrapper::InitSwapChain(const HWND& hwnd, const unsigned int w, const unsigned int h) {
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
	swapchainDesc.Width = w;
	swapchainDesc.Height = h;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.Stereo = false;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.SampleDesc.Quality = 0;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.BufferCount = 2;
	swapchainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


	auto result = mDxgiFactory->CreateSwapChainForHwnd(mCmdQueue.Get(),
		hwnd,
		&swapchainDesc,
		nullptr,
		nullptr,
		(IDXGISwapChain1 * *)mSwapChain.ReleaseAndGetAddressOf());
	assert(SUCCEEDED(result));
	return result;
}

HRESULT Dx12Wrapper::InitRenderTargets() {
	// レンダーターゲットビューの作成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = 2;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	auto result = mDev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(mRtvHeaps.ReleaseAndGetAddressOf()));

	if (FAILED(result)) {
		return result;
	}

	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = mSwapChain->GetDesc(&swcDesc);
	mBackBuffers.resize(swcDesc.BufferCount);

	D3D12_CPU_DESCRIPTOR_HANDLE handle = mRtvHeaps->GetCPUDescriptorHandleForHeapStart();

	//SRGBレンダーターゲットビュー設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	for (int i = 0; i < swcDesc.BufferCount; ++i) {
		result = mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mBackBuffers[i]));
		assert(SUCCEEDED(result));
		rtvDesc.Format = mBackBuffers[i]->GetDesc().Format;
		mDev->CreateRenderTargetView(mBackBuffers[i], &rtvDesc, handle);
		handle.ptr += mDev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}
	return result;
}

HRESULT Dx12Wrapper::InitFence() {
	auto result = mDev->CreateFence(mFenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(mFence.ReleaseAndGetAddressOf()));
	return result;
}

void Dx12Wrapper::StartDraw() {
	auto bbIdx = mSwapChain->GetCurrentBackBufferIndex();
	// バリアの設定
	D3D12_RESOURCE_BARRIER BarrierDesc = {};
	BarrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	BarrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	BarrierDesc.Transition.pResource = mBackBuffers[bbIdx];
	BarrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	BarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	BarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	mCmdList->ResourceBarrier(1, &BarrierDesc);



	auto rtvH = mRtvHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += static_cast<ULONG_PTR>(bbIdx * mDev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	mCmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);
	float clearColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	mCmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
}

Microsoft::WRL::ComPtr<ID3D12Device> Dx12Wrapper::Device() {
	return mDev;
}

void Dx12Wrapper::EndDraw() {
	// バリアの設定
	auto bbIdx = mSwapChain->GetCurrentBackBufferIndex();
	D3D12_RESOURCE_BARRIER BarrierDesc = {};
	BarrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	BarrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	BarrierDesc.Transition.pResource = mBackBuffers[bbIdx];
	BarrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	BarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	BarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	mCmdList->ResourceBarrier(1, &BarrierDesc);

	mCmdList->Close();

	ID3D12CommandList* cmdLists[] = { mCmdList.Get() };
	mCmdQueue->ExecuteCommandLists(1, cmdLists);
	// 待つ
	mCmdQueue->Signal(mFence.Get(), ++mFenceVal);
	if (mFence->GetCompletedValue() < mFenceVal) {
		auto event = CreateEvent(nullptr, false, false, nullptr);
		mFence->SetEventOnCompletion(mFenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	mCmdAllocator->Reset();
	mCmdList->Reset(mCmdAllocator.Get(), nullptr);

	mSwapChain->Present(1, 0);
}

void Dx12Wrapper::InitViewport(const int w, const int h) {
	mViewport.Width = w;//出力先の幅(ピクセル数)
	mViewport.Height = h;//出力先の高さ(ピクセル数)
	mViewport.TopLeftX = 0;//出力先の左上座標X
	mViewport.TopLeftY = 0;//出力先の左上座標Y
	mViewport.MaxDepth = 1.0f;//深度最大値
	mViewport.MinDepth = 0.0f;//深度最小値

	mScissorrect.top = 0;//切り抜き上座標
	mScissorrect.left = 0;//切り抜き左座標
	mScissorrect.right = mScissorrect.left + w;//切り抜き右座標
	mScissorrect.bottom = mScissorrect.top + h;//切り抜き下座標
}

void Dx12Wrapper::ShaderCompile() {

}