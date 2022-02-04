#include "Game.h"
// ÉGÉâÅ[èàóù
#include <cassert>
#include <algorithm>
#include "Entity.h"
#include "Hero.h"
#include "Enemy.h"
#include "Wall.h"
#include "ImguiWrapper.h"
#include "DungeonGenerator.h"
#include "Const.h"
#include "Texture.h"
#include "Input.h"
#include "CharacterManager.h"
#include "Stair.h"
#include "SpriteRenderer.h"
#include "ParameterComponent.h"
#include "Item.h"
#include "Weapon.h"
#include "Food.h"

using std::chrono::system_clock;
using std::chrono::duration_cast;

Game::Game(HINSTANCE hinst) :mUpdatingEntities(false), mIsUpdateGame(true) {
	mLastTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();// WindowÇÃçÏê¨
	mWindow = new Window();
	mWindow->Init();
	mWindow->Run();
	mInput = new Input(hinst, mWindow->GetHwnd());
	XMFLOAT3 eye(0, 0, -1);
	XMFLOAT3 target(0, 0, 0);
	XMFLOAT3 up(0, 1, 0);
	mViewMat = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	mProjMat = XMMatrixPerspectiveFovLH(
		XM_PIDIV4,
		static_cast<double>(mWindow->GetWidth()) / static_cast<double>(mWindow->GetHeight()),
		1.0f,//ãﬂÇ¢ï˚
		10.0f//âìÇ¢ï˚
	);
}

Game::~Game() {
	delete mWindow;
	delete mDx12Wrapper;
}

void Game::LoadImgFile(const wchar_t* filename) {
	Texture* texture = new Texture(this);
	texture->LoadImgFile(filename);
	// texture->CreateTexture(mWindow->GetWidth(), mWindow->GetHeight());
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

	LoadImgFile(L"Resources/Images/Wall.png");
	LoadImgFile(L"Resources/Images/blob.png");
	LoadImgFile(L"Resources/Images/stairs.png");
	LoadImgFile(L"Resources/Images/weapon.png");
	LoadImgFile(L"Resources/Images/food.png");
	LoadImgFile(L"Resources/Images/herofront1.png");
	LoadImgFile(L"Resources/Images/herofront2.png");
	LoadImgFile(L"Resources/Images/herofront3.png");

	mImguiWrapper = new ImguiWrapper(mWindow->GetHwnd(), mDx12Wrapper, mInput, this);

	mDgGen = new DungeonGenerator();
	mDgGen->createDg();
	for (int i = 0; i < mDgGen->getFloor()->data.size(); i++) {
		for (int j = 0; j < mDgGen->getFloor()->data[0].size(); j++) {
			if (mDgGen->getFloor()->data[i][j] != Const::Cell::Wall) {
				mWalls.push_back(new Wall(this, XMFLOAT3(j, i, 0)));
			}
		}
	}
	XMFLOAT2 initPos = mDgGen->getRandomPosInRoom();
	mStair = new Stair(this, XMFLOAT3(initPos.x, initPos.y, 1.0f));

	initPos = mDgGen->getRandomPosInRoom();
	mHero = new Hero(this, XMFLOAT3(initPos.x, initPos.y, 1.0f));

	mCharacterManager = new CharacterManager(mHero);

	for (int i = 0; i < Const::INIT_ENEMIES_NUM; i++) {
		initPos = mDgGen->getRandomPosInRoom();
		Enemy* blob = new Enemy(this, XMFLOAT3(initPos.x, initPos.y, 1.0f));
		mCharacterManager->AddEnemy(blob);
	}

	for (int i = 0; i < Const::INIT_ITEM_NUM; i++) {
		initPos = mDgGen->getRandomPosInRoomItem();
		Weapon* weapon = new Weapon(this, XMFLOAT3(initPos.x, initPos.y, 1.0f), Const::TexId::Weapon);
		mItems.push_back(static_cast<Item*>(weapon));
	}

	for (int i = 0; i < Const::INIT_ITEM_NUM; i++) {
		initPos = mDgGen->getRandomPosInRoomItem();
		Food* food = new Food(this, XMFLOAT3(initPos.x, initPos.y, 1.0f), Const::TexId::Food);
		mItems.push_back(static_cast<Item*>(food));
	}

	SortEntitiesByUpdateOrder();
}

void Game::InitDungeon() {
	ParameterComponent* heroParam = static_cast<ParameterComponent*>(mHero->GetComponent("ParameterComponent"));
	delete mCharacterManager;
	for (auto wall : mWalls) {
		delete wall;
	}
	for (auto item : mItems) {
		delete item;
	}
	delete mStair;
	mWalls.clear();
	mItems.clear();
	delete mDgGen;
	mDgGen = nullptr;
	mDgGen = new DungeonGenerator();
	mDgGen->createDg();

	NewFloors();

	XMFLOAT2 initPos = mDgGen->getRandomPosInRoom();
	mStair = new Stair(this, XMFLOAT3(initPos.x, initPos.y, 1.0f));

	initPos = mDgGen->getRandomPosInRoom();
	mHero->SetPosition(XMINT2(initPos.x, initPos.y));

	mCharacterManager = new CharacterManager(mHero);

	for (int i = 0; i < Const::INIT_ENEMIES_NUM; i++) {
		initPos = mDgGen->getRandomPosInRoom();
		Enemy* blob = new Enemy(this, XMFLOAT3(initPos.x, initPos.y, 1.0f));
		mCharacterManager->AddEnemy(blob);
	}

	for (int i = 0; i < Const::INIT_ITEM_NUM; i++) {
		initPos = mDgGen->getRandomPosInRoomItem();
		Weapon* weapon = new Weapon(this, XMFLOAT3(initPos.x, initPos.y, 1.0f), Const::TexId::Weapon);
		mItems.push_back(static_cast<Item*>(weapon));
	}
	for (int i = 0; i < Const::INIT_ITEM_NUM; i++) {
		initPos = mDgGen->getRandomPosInRoomItem();
		Food* food = new Food(this, XMFLOAT3(initPos.x, initPos.y, 1.0f), Const::TexId::Food);
		mItems.push_back(static_cast<Item*>(food));
	}
	SortEntitiesByUpdateOrder();

	mIsUpdateGame = true;
}


void Game::NewFloors() {
	for (int i = 0; i < mDgGen->getFloor()->data.size(); i++) {
		for (int j = 0; j < mDgGen->getFloor()->data[0].size(); j++) {
			if (mDgGen->getFloor()->data[i][j] != Const::Cell::Wall) {
				Wall* wall = new Wall(this, XMFLOAT3(j, i, 0));
				mWalls.push_back(wall);
			}
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
		if (mIsUpdateGame) {
			UpdateGame();
		}
		mImguiWrapper->Draw();
		mDx12Wrapper->EndDraw();

		// Ç∆ÇËÇ†Ç¶Ç∏
		mDgGen->SetCellType(mStair->GetPosition().x, mStair->GetPosition().y, Const::Cell::Stair);

		mCharacterManager->Update();

		if (mDgGen->getCellType(mHero->GetPosition().x, mHero->GetPosition().y) == Const::Cell::Stair) {
			mIsUpdateGame = false;
			InitDungeon();
		}

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

	// update dead entities
	std::vector<Entity*> deadEntities;

	for (auto entity : deadEntities) {
		delete entity;
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
	// if is in entities
	for (auto it = mEntities.begin(); it != mEntities.end();) {
		if (*it == entity) {
			it = mEntities.erase(it);
		}
		else {
			++it;
		}
	}
}


void Game::SortEntitiesByUpdateOrder() {
	std::sort(mEntities.begin(), mEntities.end(), [](Entity * l, Entity * r) {return l->GetUpdateOrder() < r->GetUpdateOrder(); });
}