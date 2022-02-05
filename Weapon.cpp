#include "Weapon.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "ParameterComponent.h"
#include "Hero.h"

Weapon::Weapon(Game* game, XMFLOAT3 pos, Const::TexId::Type texId) :Item(game, pos, texId), mType(Item::Type::Weapon) {
	mName = u8"‚Â‚é‚¬";
}

Weapon::~Weapon() {}

void Weapon::Adapt() {
	ParameterComponent* pc = static_cast<ParameterComponent*>(mGame->GetHero()->GetComponent("ParameterComponent"));
	pc->SetAttack(pc->GetAttack() + 4);
	mInfo = u8"‚»‚¤‚Ñ‚¿‚ã‚¤";
}
