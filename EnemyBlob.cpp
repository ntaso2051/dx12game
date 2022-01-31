#include "EnemyBlob.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "ParameterComponent.h"
#include "Const.h"
#include "DungeonGenerator.h"

EnemyBlob::EnemyBlob(Game* game, XMFLOAT3 pos) : Entity(game) {
	SpriteComponent* sc = new SpriteComponent(this, Const::TexId::Blob);
	ParameterComponent* pc = new ParameterComponent(this, Const::BLOB_INIT_HP, Const::BLOB_INIT_EXP, Const::BLOB_INIT_LEVEL, Const::BLOB_INIT_ATTACK);
	mPosition.x = pos.x;
	mPosition.y = pos.y;
	game->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Enemy);
}

EnemyBlob::~EnemyBlob() {

}

void EnemyBlob::UpdateEntity(float deltaTime) {

}

