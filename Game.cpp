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
	// Windowの作成
	mWindow = new Window();
	mWindow->Init();
	mWindow->Run();

	// DirectX12の初期化
	mDx12Wrapper = new Dx12Wrapper();
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