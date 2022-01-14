#include "Game.h"

Game::Game() {

}

Game::~Game() {
	mWindow->~Window();
}

void Game::Init() {
	mWindow = new Window();
	mWindow->Init();
	mWindow->Run();
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