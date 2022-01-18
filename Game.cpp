#include "Game.h"
#include <cassert>
#ifdef _DEBUG
#include <iostream>
#endif

Game::Game() {

}

Game::~Game() {
	mWindow->~Window();
	mDx12Wrapper->~Dx12Wrapper();
}

void Game::Init() {
	// Window‚Ìì¬
	mWindow = new Window();
	mWindow->Init();
	mWindow->Run();

	// DirectX12‚Ì‰Šú‰»
	mDx12Wrapper = new Dx12Wrapper();
#ifdef _DEBUG
	mDx12Wrapper->EnableDebugLayer();
#endif
	if (FAILED(mDx12Wrapper->InitDXGIDevice())) {
#ifdef _DEBUG
		std::cout << "Failed to initialize dxgi device" << std::endl;
#endif
	}
	if (FAILED(mDx12Wrapper->InitCommand())) {
#ifdef _DEBUG
		std::cout << "Failed to initialize dxgi command" << std::endl;
#endif
	}
	if (FAILED(mDx12Wrapper->InitSwapChain(mWindow->GetHwnd(), mWindow->GetWidth(), mWindow->GetHeight()))) {
#ifdef _DEBUG
		std::cout << "Failed to initialize swapchain" << std::endl;
#endif // _DEBUG
	}
	if (FAILED(mDx12Wrapper->InitRenderTargets())) {
#ifdef _DEBUG
		std::cout << "Failed to initialize render target view" << std::endl;
#endif
	}
	if (FAILED(mDx12Wrapper->InitFence())) {
#ifdef _DEBUG
		std::cout << "Failed to initialize fence" << std::endl;
#endif
	}

	mDx12Wrapper->DrawSqurePolygon(mWindow->GetWidth(), mWindow->GetHeight());
	mDx12Wrapper->ShaderCompile();
}

void Game::Loop() {
	MSG msg = {};
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT) {
			break;
		}
		mDx12Wrapper->StartDraw();
		mDx12Wrapper->EndDraw();
	}
}