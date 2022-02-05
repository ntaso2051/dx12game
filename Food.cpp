#include "Food.h"
#include "Game.h"
#include "SpriteComponent.h"

Food::Food(Game* game, XMFLOAT3 pos, Const::TexId::Type texId) :Item(game, pos, texId), mType(Item::Type::Once) {
	mName = u8"ВѕВс";
}

Food::~Food() {}