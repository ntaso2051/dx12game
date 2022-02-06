#include "UI.h"
#include "Game.h"
#include "UIRenderComponent.h"
#include "Const.h"
#include "AnimSpriteComponent.h"
#include "SpriteComponent.h"

UI::UI(Game* game) :Entity(game, XMFLOAT3(1.0f, 1.0f, 1.0f)) {
	const float floorUISize = 6.0f;
	mUrc = new UIRenderComponent(this, Const::TexId::UI, 1.0f, 1.0f, 0.0f, 0.0f);
	mUIB = new UIRenderComponent(this, Const::TexId::UIB, floorUISize, floorUISize, -1.5f, 0.0f);
	mUIF = new UIRenderComponent(this, Const::TexId::UIF, floorUISize, floorUISize, 2.5f, 0.0f);

	const float titleSizeX = 2.0f;
	const float titleSizeY = 6.0f;
	mTitle = new UIRenderComponent(this, Const::TexId::Title, titleSizeX, titleSizeY, 0.0f, 0.0f);

	const float titleSubSizeX = 4.0f;
	const float titleSubSizeY = 20.0f;
	mTitleSub = new UIRenderComponent(this, Const::TexId::TitleSub, titleSubSizeX, titleSubSizeY, 0.0f, -1.0f);

	for (int i = Const::TexId::UI0; i <= Const::TexId::UI9; i++) {
		UIRenderComponent* urc0 = new UIRenderComponent(this, i, floorUISize, floorUISize, 1.0f, 0.0f);
		mNum0.push_back(urc0);
		UIRenderComponent* urc1 = new UIRenderComponent(this, i, floorUISize, floorUISize, 0.0f, 0.0f);
		mNum1.push_back(urc1);
	}
	Deactivate();
	DeactivateTitle();
	mUpdateOrder = 130;
}

UI::~UI() {}

void UI::UpdateEntity(float deltaTime) {
	if (mUrc->ActiveSelf()) {
		mDisplayTime -= deltaTime;
		if (mDisplayTime < 0.0f) {
			mDisplayTime = mDisplayTimeMax;
			Deactivate();
			mGame->SetIsDisplayUI(false);
		}
	}
}

void UI::Activate(int floor) {
	mUrc->SetActive(true);
	int keta1 = floor % 10;
	int keta2 = (floor - floor % 10) / 10;
	mNum0[keta1]->SetActive(true);
	mNum1[keta2]->SetActive(true);
	mUIB->SetActive(true);
	mUIF->SetActive(true);
	mGame->SetIsDisplayUI(true);
}

void UI::Deactivate() {
	mUrc->SetActive(false);
	mUIB->SetActive(false);
	mUIF->SetActive(false);
	for (auto num0 : mNum0) {
		num0->SetActive(false);
	}
	for (auto num1 : mNum1) {
		num1->SetActive(false);
	}
}

void UI::ActivateTitle() {
	mTitle->SetActive(true);
	mTitleSub->SetActive(true);
}

void UI::DeactivateTitle() {
	mTitle->SetActive(false);
	mTitleSub->SetActive(false);
	mGame->SetIsStarted(true);
}

