#include "Stair.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "ParameterComponent.h"
#include "Const.h"
#include "DungeonGenerator.h"

Stair::Stair(Game* game, XMFLOAT3 pos) : Entity(game) {
	SpriteComponent* sc = new SpriteComponent(this, Const::TexId::STAIR, 110);
	mPosition.x = pos.x;
	mPosition.y = pos.y;
	mGame->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Stair);
	mUpdateOrder = 95;
}

Stair::~Stair() {

}