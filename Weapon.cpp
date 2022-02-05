#include "Weapon.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "ParameterComponent.h"
#include "Hero.h"

Weapon::Weapon(Game* game, XMFLOAT3 pos, Const::TexId::Type texId) :Item(game, pos, texId) {
	mName = u8"‚Â‚é‚¬";
	mType = Item::Type::Equipment;
}

Weapon::~Weapon() {}

void Weapon::Adapt() {
	if (!mGame->GetHero()->GetIsAttached(Hero::Equipment::Weapon)) {
		ParameterComponent* pc = static_cast<ParameterComponent*>(mGame->GetHero()->GetComponent("ParameterComponent"));
		pc->SetAttack(pc->GetAttack() + 4);
		mInfo = u8"‚»‚¤‚Ñ‚¿‚ã‚¤";
		mGame->GetHero()->AttachOrRemoveEquipment(Hero::Weapon, true);
	}
}

void Weapon::Deadapt() {
	if (mGame->GetHero()->GetIsAttached(Hero::Equipment::Weapon) && mInfo != "") {
		ParameterComponent* pc = static_cast<ParameterComponent*>(mGame->GetHero()->GetComponent("ParameterComponent"));
		pc->SetAttack(pc->GetAttack() - 4);
		mInfo = "";
		mGame->GetHero()->AttachOrRemoveEquipment(Hero::Equipment::Weapon, false);
	}
}

void Weapon::Remove() {
	Item::Remove();
	Deadapt();
}