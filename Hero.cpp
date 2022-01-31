#include "Hero.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "Const.h"
#include "Input.h"
#include "DungeonGenerator.h"
#include "ParameterComponent.h"
#include "CharacterManager.h"

using namespace DirectX;


Hero::Hero(Game* game, XMFLOAT3 pos) :Entity(game) {
	SpriteComponent* sc = new SpriteComponent(this, Const::TexId::Hero);
	ParameterComponent* pc = new ParameterComponent(this, Const::HERO_INIT_HP, Const::HERO_INIT_EXP, Const::HERO_INIT_LEVEL, Const::HERO_INIT_ATTACK);
	mPosition = XMINT2(pos.x, pos.y);
	mDirection = XMINT2(1, 0);
	mGame->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Hero);
}

void Hero::Attack() {
	mGame->GetChrManager()->AttackRequest(mPosition, mDirection);
}

void Hero::UpdateEntity(float deltaTime) {
	// ˆÚ“®Œ³‚ðFloor‚É•ÏX
	mGame->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Floor);
	if (
		mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::W_KEY) &&
		mGame->GetDgGen()->getCellType(mPosition.x, mPosition.y + 1) != Const::Cell::Wall &&
		mGame->GetDgGen()->getCellType(mPosition.x, mPosition.y + 1) != Const::Cell::Enemy
		) {
		mPosition.y++;
		mDirection = XMINT2(0, 1);
	}
	if (
		mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::S_KEY) &&
		mGame->GetDgGen()->getCellType(mPosition.x, mPosition.y - 1) != Const::Cell::Wall &&
		mGame->GetDgGen()->getCellType(mPosition.x, mPosition.y - 1) != Const::Cell::Enemy
		) {
		mPosition.y--;
		mDirection = XMINT2(0, -1);
	}
	if (
		mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::D_KEY) &&
		mGame->GetDgGen()->getCellType(mPosition.x + 1, mPosition.y) != Const::Cell::Wall &&
		mGame->GetDgGen()->getCellType(mPosition.x + 1, mPosition.y) != Const::Cell::Enemy
		) {
		mPosition.x++;
		mDirection = XMINT2(1, 0);
	}
	if (
		mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::A_KEY) &&
		mGame->GetDgGen()->getCellType(mPosition.x - 1, mPosition.y) != Const::Cell::Wall &&
		mGame->GetDgGen()->getCellType(mPosition.x - 1, mPosition.y) != Const::Cell::Enemy
		) {
		mPosition.x--;
		mDirection = XMINT2(-1, 0);
	}

	if (mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::SPACE_KEY)) {
		mGame->GetChrManager()->AttackRequest(mPosition, mDirection);
	}
	// ˆÚ“®æ‚ðHero‚É•ÏX
	mGame->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Hero);
	// ‚Æ‚è‚ ‚¦‚¸Rotate‚·‚é
	// mWorldMat *= XMMatrixRotationZ(deltaTime);
}

