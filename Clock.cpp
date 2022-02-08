#include "Clock.h"
#include "Game.h"
#include "Hero.h"
#include "ImguiWrapper.h"

void Clock::Init() {
	mName = u8"�Ƃ���";
	mType = Item::Type::Once;
	mDetails = u8"5�b�������~�b�g�^�C�����ӂ���";
}

Clock::Clock(Game* game, XMFLOAT3 pos, Const::TexId::Type texId) : Item(game, pos, texId) {
	Init();
}

Clock::Clock(Game* game) : Item(game) {
	Init();
}
Clock::~Clock() {}

void Clock::Adapt() {
	mGame->GetHero()->UpLimitTime(5.0f);
	mGame->GetImgui()->Cout(u8"���~�b�g�^�C�����ӂ����I");
	Remove();
}

void Clock::Deadapt() {}

void Clock::Remove() {
	Item::Remove();
	Deadapt();
}