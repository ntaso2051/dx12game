#pragma once
#include <Windows.h>
#include <dinput.h>
#include <vector>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class Input {
public:
	Input(HINSTANCE hinst, HWND hwnd);
	~Input();
	void UpdateKeyState();
	enum KEY_INFO {
		W_KEY,
		A_KEY,
		S_KEY,
		D_KEY,
		SPECE_KEY,
		MAX_KEY_INFO,
	};

	bool GetKey(KEY_INFO keyInfo);
	bool GetKeyEnter(KEY_INFO keyInfo);
	bool GetKeyExit(KEY_INFO keyInfo);

private:
	LPDIRECTINPUT8 mInterface;
	LPDIRECTINPUTDEVICE8A mDev;
	std::vector<BYTE> mKeyState;
	std::vector<BYTE> mLastKeyState;
	enum STATE {
		NONE,
		ENTER,
		PUSH,
		EXIT,
	};

	std::vector<STATE> mState;

	int mKeyInfo[KEY_INFO::MAX_KEY_INFO] = {
		DIK_W,
		DIK_A,
		DIK_S,
		DIK_D,
		DIK_SPACE,
	};
};