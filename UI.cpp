#include "UI.h"
#include "Game.h"
#include "UIRenderComponent.h"
#include "Const.h"
#include "AnimSpriteComponent.h"
#include "SpriteComponent.h"

UI::UI(Game* game) :Entity(game, XMFLOAT3(1.0f, 1.0f, 1.0f)) {
	mUrc = new UIRenderComponent(this, Const::TexId::UI, 1.0f, 1.0f, 0.0f, 0.0f);
	mUIB = new UIRenderComponent(this, Const::TexId::UIB, 6.0f, 6.0f, -1.0f, 0.0f);
	mUIF = new UIRenderComponent(this, Const::TexId::UIF, 6.0f, 6.0f, 2.0f, 0.0f);
	for (int i = Const::TexId::UI0; i <= Const::TexId::UI9; i++) {
		UIRenderComponent* urc0 = new UIRenderComponent(this, i, 6.0f, 6.0f, 1.0f, 0.0f);
		mNum0.push_back(urc0);
		UIRenderComponent* urc1 = new UIRenderComponent(this, i, 6.0f, 6.0f, 0.0f, 0.0f);
		mNum1.push_back(urc1);
	}
	Deactivate();
	mUpdateOrder = 130;
}

UI::~UI() {}

void UI::UpdateEntity(float deltaTime) {
	if (mUrc->ActiveSelf()) {
		mDisplayTime -= deltaTime;
		if (mDisplayTime < 0.0f) {
			mDisplayTime = mDisplayTimeMax;
			Deactivate();
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