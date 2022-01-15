#include "Game.h"
#ifdef _DEBUG
#include <iostream>
#endif

Game::Game() {

}

Game::~Game() {
	mWindow->~Window();
}

void Game::Init() {
	mWindow = new Window();
	mWindow->Init();
	mWindow->Run();

	mDx12Wrapper = new Dx12Wrapper();
	if (SUCCEEDED(mDx12Wrapper->InitDXGIDevice())) {
#ifdef _DEBUG
		std::cout << "Success initialize dxgi device" << std::endl;
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
	}
}