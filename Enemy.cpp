#include "Enemy.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "ParameterComponent.h"
#include "Const.h"
#include "DungeonGenerator.h"

Enemy::Enemy(Game* game, XMFLOAT3 pos) : Entity(game, pos) {
	SpriteComponent* sc = new SpriteComponent(this, Const::TexId::Blob);
	ParameterComponent* pc = new ParameterComponent(this, Const::BLOB_INIT_HP, Const::BLOB_INIT_EXP, Const::BLOB_INIT_LEVEL, Const::BLOB_INIT_ATTACK);
	mGame->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Enemy);
	mUpdateOrder = 110;
}

Enemy::~Enemy() {

}


void Enemy::UpdateEntity(float deltaTime) {

}