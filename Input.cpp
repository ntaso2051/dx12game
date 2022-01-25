#include "Input.h"
#include <cassert>

Input::Input(HINSTANCE hinst, HWND hwnd) {
	HRESULT result = DirectInput8Create(
		hinst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)& mInterface,
		nullptr
	);
	if (FAILED(result)) {
		assert(0);
	}
	result = mInterface->CreateDevice(
		GUID_SysKeyboard,
		&mDev,
		nullptr
	);
	if (FAILED(result)) {
		assert(0);
	}
	result = mDev->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result)) {
		assert(0);
	}
	result = mDev->SetCooperativeLevel(
		hwnd,
		DISCL_BACKGROUND | DISCL_NONEXCLUSIVE
	);
	if (FAILED(result)) {
		assert(0);
	}
	result = mDev->GetDeviceState(256, mKeyState);
}

Input::~Input() {
	mDev->Unacquire();
	mDev->Release();
	mInterface->Release();
}

void Input::UpdateKeyState() {
	mDev->GetDeviceState(256, mKeyState);
}

bool Input::GetKey(KEY_INFO keyInfo) {
	return (mKeyState[keyInfo] == STATE::PUSH);
}

bool Input::GetKeyEnter(KEY_INFO keyInfo) {
	return (mKeyState[keyInfo] == STATE::ENTER);
}

bool Input::GetKeyExit(KEY_INFO keyInfo) {
	return (mKeyState[keyInfo] == STATE::EXIT);
}