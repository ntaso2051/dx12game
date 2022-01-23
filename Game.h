#pragma once
#include "Window.h"
#include "Dx12Wrapper.h"
#include "SpriteRenderer.h"
#include <vector>
// éûçèä÷òA
#include <chrono>
#include <ctime>

using std::chrono::milliseconds;

class Game {
public:
	Game();
	~Game();
	void Init();
	void Loop();

	void AddEntity(class Entity* entity);
	void RemoveEntity(class Entity* entity);
	Dx12Wrapper& GetDx12() { return *mDx12Wrapper; }
	Window* GetWindow() { return mWindow; }
private:
	void UpdateGame();
	Window* mWindow;
	Dx12Wrapper* mDx12Wrapper;

	bool mUpdatingEntities;
	std::vector<class Entity*> mEntities;
	std::vector<class Entity*> mPendingEntities;

	int64_t mLastTime;

	class Hero* mHero;
};