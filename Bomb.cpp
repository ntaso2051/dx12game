#include "Bomb.h"
#include "Game.h"
#include "ParameterComponent.h"
#include "CharacterManager.h"
#include "Hero.h"
#include "Enemy.h"
#include "ImguiWrapper.h"

void Bomb::Init() {
	mName = u8"�΂�����";
	mType = Item::Type::Once;
	mDetails = u8"�t���A�̂Ă���HP�����ׂ�1�ɂ���D�݂�������_���[�W��������D";
}

Bomb::Bomb(Game* game, XMFLOAT3 pos, Const::TexId::Type texId) : Item(game, pos, texId) {
	Init();
}

Bomb::Bomb(Game* game) : Item(game) {
	Init();
}

Bomb::~Bomb() {}

void Bomb::Adapt() {
	ParameterComponent* pc = static_cast<ParameterComponent*>(mGame->GetHero()->GetComponent("ParameterComponent"));
	pc->SetHp(1);
	auto enemies = mGame->GetChrManager()->GetEnemies();
	for (auto e : enemies) {
		ParameterComponent* epc = static_cast<ParameterComponent*>(e->GetComponent("ParameterComponent"));
		epc->SetHp(1);
	}
	mGame->GetImgui()->Cout(u8"�΂����񂪂΂��͂����I�t���A�̂Ă���HP��1�ɂȂ����I");
	Remove();
}


void Bomb::Deadapt() {

}

void Bomb::Remove() {
	Item::Remove();
	Deadapt();
}
