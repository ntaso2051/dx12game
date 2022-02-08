#include "Herbs.h"
#include "Game.h"
#include "Hero.h"
#include "ParameterComponent.h"
#include "ImguiWrapper.h"

void Herbs::Init() {
	mName = u8"�₭����";
	mType = Item::Type::Once;
	mDetails = u8"HP�������ӂ�����";
}

Herbs::Herbs(Game* game, XMFLOAT3 pos, Const::TexId::Type texId) :Item(game, pos, texId) {
	Init();
}

Herbs::Herbs(Game* game) : Item(game) {
	Init();
}

Herbs::~Herbs() {}

void Herbs::Adapt() {
	ParameterComponent* pc = static_cast<ParameterComponent*>(mGame->GetHero()->GetComponent("ParameterComponent"));
	pc->SetHp((pc->GetHp() + mHealHp > pc->GetMaxHp()) ? pc->GetMaxHp() : pc->GetHp() + mHealHp);
	mGame->GetImgui()->Cout(u8"HP�������ӂ������I");
	Remove();
}

void Herbs::Deadapt() {}

void Herbs::Remove() {
	Item::Remove();
	Deadapt();
}