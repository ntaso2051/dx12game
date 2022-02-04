#include "Weapon.h"
#include "Game.h"
#include "SpriteComponent.h"

Weapon::Weapon(Game* game, XMFLOAT3 pos, Const::TexId::Type texId) :Item(game, pos, texId), mType(Item::Type::Weapon) {

}

Weapon::~Weapon() {}