#include "Food.h"
#include "Game.h"
#include "SpriteComponent.h"

Food::Food(Game* game, XMFLOAT3 pos, Const::TexId::Type texId) :Item(game, pos, texId) {
	mName = u8"�ς�";
	mType = Item::Type::Once;
}

Food::~Food() {}