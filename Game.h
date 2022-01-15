#pragma once
#include "Window.h"
#include "Dx12Wrapper.h"

class Game {
public:
	Game();
	~Game();
	void Init();
	void Loop();
private:
	Window* mWindow;
	Dx12Wrapper* mDx12Wrapper;
};