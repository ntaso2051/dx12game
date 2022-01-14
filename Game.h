#pragma once
#include "Window.h"

class Game {
public:
	Game();
	~Game();
	void Init();
	void Loop();
private:
	Window* mWindow;
};