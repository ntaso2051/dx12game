#include "Enemy.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "ParameterComponent.h"
#include "Const.h"
#include "DungeonGenerator.h"

Enemy::Enemy(Game* game, XMFLOAT3 pos) : Entity(game) {
	SpriteComponent* sc = new SpriteComponent(this, Const::TexId::Blob);
	ParameterComponent* pc = new ParameterComponent(this, Const::BLOB_INIT_HP, Const::BLOB_INIT_EXP, Const::BLOB_INIT_LEVEL, Const::BLOB_INIT_ATTACK);
	mPosition.x = pos.x;
	mPosition.y = pos.y;
	mGame->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Enemy);
}

Enemy::~Enemy() {

}


void Enemy::UpdateEntity(float deltaTime) {

}