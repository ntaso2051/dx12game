#pragma once
#include "Window.h"
#include "Dx12Wrapper.h"
#include "SpriteRenderer.h"
#include <vector>
#include <DirectXMath.h>
// �����֘A
#include <chrono>
#include <ctime>

#include "Const.h"

using std::chrono::milliseconds;

class Game {
public:
	Game(HINSTANCE hinst);
	~Game();
	void Init();
	void InitDungeon();
	void NewFloors();
	void Loop();

	void AddEntity(class Entity* entity);
	void RemoveEntity(class Entity* entity);
	void SortEntitiesByUpdateOrder();
	Dx12Wrapper& GetDx12() { return *mDx12Wrapper; }
	Window* GetWindow() { return mWindow; }
	class Input* GetInput() { return mInput; }
	class DungeonGenerator* GetDgGen() { return mDgGen; }
	class CharacterManager* GetChrManager() { return mCharacterManager; }
	class ImguiWrapper* GetImgui() { return mImguiWrapper; }

	void LoadImgFile(const wchar_t* filename);
	class Texture* GetTexture() { return mTextures[0]; }
	class Texture* GetTextureById(int id) { return mTextures[id]; }
	XMMATRIX GetViewMat() { return mViewMat; }
	XMMATRIX GetProjMat() { return mProjMat; }
	class Hero* GetHero() { return mHero; }
	bool GetUpdatingEntities() { return mUpdatingEntities; }

	void RemoveFallenItem(class Item*);
private:
	void UpdateGame();
	Window* mWindow;
	Dx12Wrapper* mDx12Wrapper;
	class Input* mInput;
	class ImguiWrapper* mImguiWrapper;
	class DungeonGenerator* mDgGen;

	bool mUpdatingEntities;
	bool mIsUpdateGame;
	std::vector<class Entity*> mEntities;
	std::vector<class Entity*> mPendingEntities;

	int64_t mLastTime;

	std::vector<class Texture*> mTextures;

	class CharacterManager* mCharacterManager;
	class Hero* mHero;
	class Stair* mStair;
	std::vector<class Wall*> mWalls;
	std::vector<class Item*> mFallenItems;

	XMMATRIX mViewMat;
	XMMATRIX mProjMat;
};