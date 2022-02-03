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


Hero::Hero(Game* game, XMFLOAT3 pos) :Entity(game, pos), mIsMoving(false), mDirection(XMINT2(0, -1)), mPrePos(XMINT2(pos.x, pos.y)) {
	SpriteComponent* sc = new SpriteComponent(this, Const::TexId::Hero);
	ParameterComponent* pc = new ParameterComponent(this, Const::HERO_INIT_HP, Const::HERO_INIT_EXP, Const::HERO_INIT_LEVEL, Const::HERO_INIT_ATTACK);
	mGame->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Hero);
	mUpdateOrder = 100;
}

Hero::Hero(Game* game, XMFLOAT3 pos, ParameterComponent* param) : Entity(game, pos), mIsMoving(false), mDirection(XMINT2(0, -1)), mPrePos(XMINT2(pos.x, pos.y)) {
	SpriteComponent* sc = new SpriteComponent(this, Const::TexId::Hero);
	AddComponent(param);
	mDirection = XMINT2(0, -1);
	mGame->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Hero);
}

Hero::~Hero() {}

void Hero::Attack() {
	mGame->GetChrManager()->AttackRequest(mPosition, mDirection);
}

void Hero::SetPosition(XMINT2 pos) {
	mPrePos = pos;
	mPosition.x = pos.x;
	mPosition.y = pos.y;
	mGame->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Hero);
}

void Hero::UpdateEntity(float deltaTime) {
	// ˆÚ“®Œ³‚ðFloor‚É•ÏX
	mGame->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Floor);
	// “ü—Íˆ—
	if (!mIsMoving && mGame->GetChrManager()->GetPhase() == CharacterManager::Phase::HeroPhase) {
		if (mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::W_KEY)) {
			mDirection = XMINT2(0, 1);
			mIsMoving = true;
		}
		if (mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::S_KEY)) {
			mDirection = XMINT2(0, -1);
			mIsMoving = true;
		}
		if (mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::D_KEY)) {
			mDirection = XMINT2(1, 0);
			mIsMoving = true;
		}
		if (mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::A_KEY)) {
			mDirection = XMINT2(-1, 0);
			mIsMoving = true;
		}

		if (mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::Z_KEY)) {
			mGame->GetChrManager()->AttackRequest(mPosition, mDirection);
			// ƒ^[ƒ“‚ð“G‚É“n‚·
			mGame->GetChrManager()->ChangePhase();
		}
	}

	if (
		(mIsMoving) &&
		mGame->GetDgGen()->getCellType(mPrePos.x + mDirection.x, mPrePos.y + mDirection.y) != Const::Cell::Wall &&
		mGame->GetDgGen()->getCellType(mPrePos.x + mDirection.x, mPrePos.y + mDirection.y) != Const::Cell::Enemy
		) {
		mPosition.x += mMoveSpeed * deltaTime * mDirection.x;
		mPosition.y += mMoveSpeed * deltaTime * mDirection.y;
	}
	else {
		mIsMoving = false;
	}

	if (mIsMoving && (std::abs(mPosition.x - mPrePos.x) >= 0.999f || std::abs(mPosition.y - mPrePos.y) >= 0.999f)) {
		mPosition.x = mPrePos.x + mDirection.x;
		mPosition.y = mPrePos.y + mDirection.y;
		mPrePos = XMINT2(mPosition.x, mPosition.y);
		mIsMoving = false;
		// ƒ^[ƒ“‚ð“G‚É“n‚·
		mGame->GetChrManager()->ChangePhase();
	}



	// ˆÚ“®æ‚ðHero‚É•ÏX
	mGame->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Hero);
}

