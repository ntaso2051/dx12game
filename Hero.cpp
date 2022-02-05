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
#include "AnimSpriteComponent.h"
#include "Item.h"
#include <typeinfo>

using namespace DirectX;


Hero::Hero(Game* game, XMFLOAT3 pos) :Entity(game, pos), mDirection(XMINT2(0, -1)), mPrePos(XMINT2(pos.x, pos.y)), mState(Const::State::Idle), mIsAttached(std::vector<bool>(Equipment::Max, false)), mActCnt(0) {
	std::vector<int> texIds = {
		Const::TexId::HFront1,
		Const::TexId::HFront2,
		Const::TexId::HFront3,
		Const::TexId::HFront2,
	};
	SpriteComponent* sc = new SpriteComponent(this, Const::TexId::HFront1);
	AnimSpriteComponent* asc = new AnimSpriteComponent(this, texIds);
	ParameterComponent* pc = new ParameterComponent(this, Const::HERO_INIT_HP, Const::HERO_INIT_EXP, Const::HERO_INIT_LEVEL, Const::HERO_INIT_ATTACK);
	mGame->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Hero);
	mUpdateOrder = 100;
}

Hero::Hero(Game* game, XMFLOAT3 pos, ParameterComponent* param) : Entity(game, pos), mDirection(XMINT2(0, -1)), mPrePos(XMINT2(pos.x, pos.y)), mState(Const::State::Idle), mIsAttached(std::vector<bool>(Equipment::Max, false)), mActCnt(0) {
	SpriteComponent* sc = new SpriteComponent(this, Const::TexId::HFront1);
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
	// 移動元をFloorに変更
	mGame->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Floor);
	// 入力処理
	if (mState == Const::State::Idle && mGame->GetChrManager()->GetPhase() == CharacterManager::Phase::HeroPhase) {
		if (mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::W_KEY)) {
			mDirection = XMINT2(0, 1);
			mState = Const::State::Move;
		}
		if (mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::S_KEY)) {
			mDirection = XMINT2(0, -1);
			mState = Const::State::Move;
		}
		if (mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::D_KEY)) {
			mDirection = XMINT2(1, 0);
			mState = Const::State::Move;
		}
		if (mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::A_KEY)) {
			mDirection = XMINT2(-1, 0);
			mState = Const::State::Move;
		}

		if (mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::Z_KEY)) {
			mGame->GetChrManager()->AttackRequest(mPosition, mDirection);
			// ターンを敵に渡す
			// アニメーションを付けるならAttackにする
			mState = Const::State::Idle;
			mGame->GetChrManager()->ChangePhase();
		}
	}

	if (
		// 動いているか
		(mState == Const::State::Move) &&
		// 移動先が壁か敵ではないか
		mGame->GetDgGen()->getCellType(mPrePos.x + mDirection.x, mPrePos.y + mDirection.y) != Const::Cell::Wall &&
		mGame->GetDgGen()->getCellType(mPrePos.x + mDirection.x, mPrePos.y + mDirection.y) != Const::Cell::Enemy
		) {
		mPosition.x += mMoveSpeed * deltaTime * mDirection.x;
		mPosition.y += mMoveSpeed * deltaTime * mDirection.y;
	}
	else {
		mState = Const::State::Idle;
	}

	if (mState == Const::State::Move && (std::abs(mPosition.x - mPrePos.x) >= 0.999f || std::abs(mPosition.y - mPrePos.y) >= 0.999f)) {
		mPosition.x = mPrePos.x + mDirection.x;
		mPosition.y = mPrePos.y + mDirection.y;
		mPrePos = XMINT2(mPosition.x, mPosition.y);
		mState = Const::State::Idle;
		// ターンを敵に渡す
		mGame->GetChrManager()->ChangePhase();
	}



	// 移動先をHeroに変更
	mGame->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Hero);
}

void Hero::RemoveItem(Item * item) {
	for (auto it = mItems.begin(); it != mItems.end();) {
		if (*it == item) {
			it = mItems.erase(it);
		}
		else {
			++it;
		}
	}
}

void Hero::UseItem(Item * item) {

}