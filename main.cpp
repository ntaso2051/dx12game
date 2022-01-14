#include <Windows.h>
#include "Game.h"

using namespace std;

#ifdef _DEBUG
int main() {
#else
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#endif
	Game* game = new Game();
	game->Init();
	game->Loop();
	game->~Game();
}