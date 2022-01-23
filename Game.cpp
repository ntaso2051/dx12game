#include "Game.h"
// ÉGÉâÅ[èàóù
#include <cassert>
#include "Entity.h"
#include "Hero.h"
#include "Wall.h"

#ifdef _DEBUG
#include <iostream>
#endif

using std::chrono::system_clock;
using std::chrono::duration_cast;

Game::Game() :mUpdatingEntities(false) {
	mLastTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

Game::~Game() {
	mWindow->~Window();
	mDx12Wrapper->~Dx12Wrapper();
}

void Game::Init() {
	// WindowÇÃçÏê¨
	mWindow = new Window();
	mWindow->Init();
	mWindow->Run();

	// DirectX12ÇÃèâä˙âª
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

	mDx12Wrapper->InitViewport(mWindow->GetWidth(), mWindow->GetHeight());
	mHero = new Hero(this);
	mWall = new Wall(this);
	/*
	mSpriteRenderer = new SpriteRenderer(*mDx12Wrapper);
	mSpriteRenderer->InitView(mWindow->GetWidth(), mWindow->GetHeight());
	mSpriteRenderer->CompileShader();
	mSpriteRenderer->InitRootSignature();
	mSpriteRenderer->InitGraphicPipeline();
	mSpriteRenderer->CreateTexture(mWindow->GetWidth(), mWindow->GetHeight());
	*/
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
		UpdateGame();
		mDx12Wrapper->EndDraw();
	}
}

void Game::UpdateGame() {
	auto now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	float deltaTime = (now - mLastTime) / 1000.0f;
#ifdef _DEBUG
	// std::cout << deltaTime << std::endl;
#endif
	 // update entities

	mUpdatingEntities = true;
	for (auto entity : mEntities) {
		entity->Update(deltaTime);
	}
	mUpdatingEntities = false;

	// update pending entities
	for (auto pending : mPendingEntities) {
		mEntities.emplace_back(pending);
	}
	mPendingEntities.clear();

	// update dead entities
	std::vector<Entity*> deadActors;
	for (auto entity : mEntities) {
		/*TODO:EntityÇ…StateÇí«â¡
			deadActors.emplace_back(actor);
		*/
	}

	for (auto actor : deadActors) {
		delete actor;
	}
	mLastTime = now;
}

void Game::AddEntity(Entity* entity) {
	if (mUpdatingEntities) {
		mPendingEntities.emplace_back(entity);
	}
	else {
		mEntities.emplace_back(entity);
	}
}

void Game::RemoveEntity(Entity* entity) {
	// if is in pending actors
	auto iter = std::find(mPendingEntities.begin(), mPendingEntities.end(), entity);
	if (iter != mPendingEntities.end()) {
		std::iter_swap(iter, mPendingEntities.end() - 1);
		mPendingEntities.pop_back();
	}

	// if is in actors
	iter = std::find(mEntities.begin(), mEntities.end(), entity);
	if (iter != mEntities.end()) {
		std::iter_swap(iter, mEntities.end() - 1);
		mEntities.pop_back();
	}
}