#include <Windows.h>
#include "Game.h"
#include "Input.h"

using namespace std;

int WINAPI WinMain(HINSTANCE hinst, HINSTANCE, LPSTR, int) {
	Game* game = new Game(hinst);
	game->Init();
	game->Loop();
	game->~Game();
}