#include "Game.h"
// ÉGÉâÅ[èàóù
#include <cassert>
#include "Entity.h"
#include "Hero.h"
#include "Wall.h"
#include "ImguiWrapper.h"
#include "DungeonGenerator.h"
#include "Const.h"
#include "Texture.h"
#include "Input.h"

using std::chrono::system_clock;
using std::chrono::duration_cast;

Game::Game(HINSTANCE hinst) :mUpdatingEntities(false) {
	mLastTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();// WindowÇÃçÏê¨
	mWindow = new Window();
	mWindow->Init();
	mWindow->Run();
	mInput = new Input(hinst, mWindow->GetHwnd());
}

Game::~Game() {
	mWindow->~Window();
	mDx12Wrapper->~Dx12Wrapper();
}

void Game::LoadImgFile(const wchar_t* filename) {
	Texture* texture = new Texture();
	texture->LoadImgFile(filename);
	mTextures.push_back(texture);
}

void Game::Init() {
	// DirectX12ÇÃèâä˙âª
	mDx12Wrapper = new Dx12Wrapper();
#ifdef _DEBUG
	mDx12Wrapper->EnableDebugLayer();
#endif
	if (FAILED(mDx12Wrapper->InitDXGIDevice())) {
	}
	if (FAILED(mDx12Wrapper->InitCommand())) {
	}
	if (FAILED(mDx12Wrapper->InitSwapChain(mWindow->GetHwnd(), mWindow->GetWidth(), mWindow->GetHeight()))) {
	}
	if (FAILED(mDx12Wrapper->InitRenderTargets())) {
	}
	if (FAILED(mDx12Wrapper->InitFence())) {
	}
	if (FAILED(mDx12Wrapper->InitVbIbForSprite())) {
	}

	mDx12Wrapper->InitViewport(mWindow->GetWidth(), mWindow->GetHeight());
	mDx12Wrapper->CompileShader();

	if (FAILED(mDx12Wrapper->InitRootsignatureForSprite())) {
	}
	if (FAILED(mDx12Wrapper->InitGraphicPipelineForSprite())) {
	}

	LoadImgFile(L"Resources/Images/myicon.png");
	LoadImgFile(L"Resources/Images/Wall.png");

	mImguiWrapper = new ImguiWrapper(mWindow->GetHwnd(), mDx12Wrapper);
	mHero = new Hero(this, XMFLOAT3(1.0f, 1.0f, 1.0f));
	mDgGen = new DungeonGenerator();
	mDgGen->createDg();
	mDgGen->draw();
	for (int i = 0; i < mDgGen->getFloor()->data.size(); i++) {
		for (int j = 0; j < mDgGen->getFloor()->data[0].size(); j++) {
			if (mDgGen->getFloor()->data[i][j] == Const::Cell::Wall)
				Wall * wall = new Wall(this, XMFLOAT3(j - 15, i - 15, 0));
		}
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
		mInput->UpdateKeyState();
		mDx12Wrapper->StartDraw();
		UpdateGame();
		mImguiWrapper->Draw();
		mDx12Wrapper->EndDraw();
	}
}

void Game::UpdateGame() {
	auto now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	float deltaTime = (now - mLastTime) / 1000.0f;
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

void Game::AddEntity(Entity * entity) {
	if (mUpdatingEntities) {
		mPendingEntities.emplace_back(entity);
	}
	else {
		mEntities.emplace_back(entity);
	}
}

void Game::RemoveEntity(Entity * entity) {
	// if is in pending entities
	auto iter = std::find(mPendingEntities.begin(), mPendingEntities.end(), entity);
	if (iter != mPendingEntities.end()) {
		std::iter_swap(iter, mPendingEntities.end() - 1);
		mPendingEntities.pop_back();
	}

	// if is in entities
	iter = std::find(mEntities.begin(), mEntities.end(), entity);
	if (iter != mEntities.end()) {
		std::iter_swap(iter, mEntities.end() - 1);
		mEntities.pop_back();
	}
}