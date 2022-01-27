#include "Input.h"
#include <cassert>
#include <memory>

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
	result = mDev->Acquire();
	if (FAILED(result)) {
		assert(0);
	}

	mKeyState.resize(256);
	mLastKeyState.resize(256);
	mState.resize(KEY_INFO::MAX_KEY_INFO);
}

Input::~Input() {
	mDev->Unacquire();
	mDev->Release();
	mInterface->Release();
}

void Input::UpdateKeyState() {
	for (int i = 0; i < 256; i++) {
		mLastKeyState[i] = mKeyState[i];
	}
	HRESULT result = mDev->GetDeviceState(256, mKeyState.data());
	if (FAILED(result)) {
		assert(0);
	}
	for (int i = 0; i < KEY_INFO::MAX_KEY_INFO; i++) {
		if ((mLastKeyState[mKeyInfo[i]] & 0x80) && !mKeyState[mKeyInfo[i]]) {
			mState[i] = STATE::EXIT;
		}
		if (!mLastKeyState[mKeyInfo[i]] && (mKeyState[mKeyInfo[i]] & 0x80)) {
			mState[i] = STATE::ENTER;
		}
		if ((mLastKeyState[mKeyInfo[i]] & 0x80) && (mKeyState[mKeyInfo[i]] & 0x80)) {
			mState[i] = STATE::PUSH;
		}
		if (!mLastKeyState[mKeyInfo[i]] && !mKeyState[mKeyInfo[i]]) {
			mState[i] = STATE::NONE;
		}
	}
}

bool Input::GetKey(KEY_INFO keyInfo) {
	return (mState[keyInfo] == STATE::PUSH);
}

bool Input::GetKeyEnter(KEY_INFO keyInfo) {
	return (mState[keyInfo] == STATE::ENTER);
}

bool Input::GetKeyExit(KEY_INFO keyInfo) {
	return (mState[keyInfo] == STATE::EXIT);
}