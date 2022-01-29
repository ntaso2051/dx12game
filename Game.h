#pragma once
#include "Window.h"
#include "Dx12Wrapper.h"
#include "SpriteRenderer.h"
#include <vector>
#include <DirectXMath.h>
// éûçèä÷òA
#include <chrono>
#include <ctime>

using std::chrono::milliseconds;

class Game {
public:
	Game(HINSTANCE hinst);
	~Game();
	void Init();
	void Loop();

	void AddEntity(class Entity* entity);
	void RemoveEntity(class Entity* entity);
	Dx12Wrapper& GetDx12() { return *mDx12Wrapper; }
	Window* GetWindow() { return mWindow; }
	class Input* GetInput() { return mInput; }
	void LoadImgFile(const wchar_t* filename);
	class Texture* GetTexture() { return mTextures[0]; }
	class Texture* GetTextureById(int id) { return mTextures[id]; }
	XMMATRIX GetViewMat() { return mViewMat; }
	XMMATRIX GetProjMat() { return mProjMat; }
private:
	void UpdateGame();
	Window* mWindow;
	Dx12Wrapper* mDx12Wrapper;
	class Input* mInput;
	class ImguiWrapper* mImguiWrapper;
	class DungeonGenerator* mDgGen;

	bool mUpdatingEntities;
	std::vector<class Entity*> mEntities;
	std::vector<class Entity*> mPendingEntities;

	int64_t mLastTime;

	std::vector<class Texture*> mTextures;
	class Hero* mHero;
	class Wall* mWall;

	XMMATRIX mViewMat;
	XMMATRIX mProjMat;
};