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
#include "ImguiWrapper.h"
#include "SaveData.h"
#include "Weapon.h"

using namespace DirectX;


Hero::Hero(Game* game, XMFLOAT3 pos) :Entity(game, pos), mDirection(XMINT2(0, -1)), mPrePos(XMINT2(pos.x, pos.y)), mState(Const::State::Idle), mIsAttached(std::vector<bool>(Equipment::Max, false)), mActCnt(0) {
	std::vector<int> texIds = {
		Const::TexId::HFront1,
		Const::TexId::HFront2,
		Const::TexId::HFront3,
		Const::TexId::HFront2,
	};
	std::vector<int> texIds2 = {
		Const::TexId::HRight1,
		Const::TexId::HRight2,
		Const::TexId::HRight3,
		Const::TexId::HRight2,
	};
	std::vector<int> texIds3 = {
		Const::TexId::HLeft1,
		Const::TexId::HLeft2,
		Const::TexId::HLeft3,
		Const::TexId::HLeft2,
	};
	std::vector<int> texIds4 = {
		Const::TexId::HBack1,
		Const::TexId::HBack2,
		Const::TexId::HBack3,
		Const::TexId::HBack2,
	};
	std::vector<int> texIds5 = {
		Const::TexId::HFrontRight1,
		Const::TexId::HFrontRight2,
		Const::TexId::HFrontRight3,
		Const::TexId::HFrontRight2,
	};
	std::vector<int> texIds6 = {
		Const::TexId::HFrontLeft1,
		Const::TexId::HFrontLeft2,
		Const::TexId::HFrontLeft3,
		Const::TexId::HFrontLeft2,
	};
	std::vector<int> texIds7 = {
		Const::TexId::HBackRight1,
		Const::TexId::HBackRight2,
		Const::TexId::HBackRight3,
		Const::TexId::HBackRight2,
	};
	std::vector<int> texIds8 = {
		Const::TexId::HBackLeft1,
		Const::TexId::HBackLeft2,
		Const::TexId::HBackLeft3,
		Const::TexId::HBackLeft2,
	};
	std::vector<std::vector<int>> texIdss = {
		texIds, texIds2, texIds3, texIds4, texIds5, texIds6, texIds7, texIds8,
	};
	SpriteComponent* sc = new SpriteComponent(this, Const::TexId::HFront1);
	mASC = new AnimSpriteComponent(this, texIdss);
	mPC = new ParameterComponent(this, Const::HERO_INIT_HP, Const::HERO_INIT_EXP, Const::HERO_INIT_LEVEL, Const::HERO_INIT_ATTACK);
	mGame->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Hero);
	mUpdateOrder = 100;
}

void Hero::ReadFromSaveData(SaveData* savedata) {
	SaveData::Data d = savedata->GetData();
	mPC->SetMaxHp(d.maxHp);
	mPC->SetHp(d.hp);
	mPC->SetExp(d.exp);
	mPC->SetLevel(d.level);
	mPC->SetHunger(d.hunger);
	mPC->SetAttack(d.attack);
}

Hero::Hero(Game* game, XMFLOAT3 pos, ParameterComponent* param) : Entity(game, pos), mDirection(XMINT2(0, -1)), mPrePos(XMINT2(pos.x, pos.y)), mState(Const::State::Idle), mIsAttached(std::vector<bool>(Equipment::Max, false)), mActCnt(0) {
	SpriteComponent* sc = new SpriteComponent(this, Const::TexId::HFront1);
	AddComponent(param);
	mDirection = XMINT2(0, -1);
	mGame->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Hero);
}

Hero::~Hero() {
	delete mASC;
}

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
	if (!mGame->GetIsStarted() || mGame->GetIsDisplayUI()) return;
	// 移動元をFloorに変更
	mGame->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Floor);
	// 入力処理
	if (mState == Const::State::Idle && mGame->GetChrManager()->GetPhase() == CharacterManager::Phase::HeroPhase) {
		if (!mGame->GetInput()->GetKey(Input::KEY_INFO::SHIFT_KEY)) {
			if (mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::W_KEY)) {
				mDirection = XMINT2(0, 1);
				mASC->SetId(Dir::Back);
				mState = Const::State::Move;
			}
			if (mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::S_KEY)) {
				mDirection = XMINT2(0, -1);
				mASC->SetId(Dir::Front);
				mState = Const::State::Move;
			}
			if (mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::D_KEY)) {
				mDirection = XMINT2(1, 0);
				mASC->SetId(Dir::Left);
				mState = Const::State::Move;
			}
			if (mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::A_KEY)) {
				mDirection = XMINT2(-1, 0);
				mASC->SetId(Dir::Right);
				mState = Const::State::Move;
			}
		}
		// シフトを押しながらだと向きのみ変えられる．
		if (mGame->GetInput()->GetKey(Input::KEY_INFO::SHIFT_KEY)) {
			if (
				mGame->GetInput()->GetKey(Input::KEY_INFO::W_KEY) &&
				!mGame->GetInput()->GetKey(Input::KEY_INFO::A_KEY) &&
				!mGame->GetInput()->GetKey(Input::KEY_INFO::S_KEY) &&
				!mGame->GetInput()->GetKey(Input::KEY_INFO::D_KEY)
				) {
				mDirection = XMINT2(0, 1);
				mASC->SetId(Dir::Back);
			}
			if (
				mGame->GetInput()->GetKey(Input::KEY_INFO::S_KEY) &&
				!mGame->GetInput()->GetKey(Input::KEY_INFO::A_KEY) &&
				!mGame->GetInput()->GetKey(Input::KEY_INFO::W_KEY) &&
				!mGame->GetInput()->GetKey(Input::KEY_INFO::D_KEY)
				) {
				mDirection = XMINT2(0, -1);
				mASC->SetId(Dir::Front);
			}
			if (
				mGame->GetInput()->GetKey(Input::KEY_INFO::D_KEY) &&
				!mGame->GetInput()->GetKey(Input::KEY_INFO::A_KEY) &&
				!mGame->GetInput()->GetKey(Input::KEY_INFO::S_KEY) &&
				!mGame->GetInput()->GetKey(Input::KEY_INFO::W_KEY)
				) {
				mDirection = XMINT2(1, 0);
				mASC->SetId(Dir::Left);
			}
			if (
				mGame->GetInput()->GetKey(Input::KEY_INFO::A_KEY) &&
				!mGame->GetInput()->GetKey(Input::KEY_INFO::W_KEY) &&
				!mGame->GetInput()->GetKey(Input::KEY_INFO::S_KEY) &&
				!mGame->GetInput()->GetKey(Input::KEY_INFO::D_KEY)
				) {
				mDirection = XMINT2(-1, 0);
				mASC->SetId(Dir::Right);
			}
			if (
				mGame->GetInput()->GetKey(Input::KEY_INFO::W_KEY) &&
				mGame->GetInput()->GetKey(Input::KEY_INFO::D_KEY) &&
				!mGame->GetInput()->GetKey(Input::KEY_INFO::A_KEY) &&
				!mGame->GetInput()->GetKey(Input::KEY_INFO::S_KEY)
				) {
				mDirection = XMINT2(1, 1);
				mASC->SetId(Dir::BackRight);
			}
			if (
				mGame->GetInput()->GetKey(Input::KEY_INFO::W_KEY) &&
				mGame->GetInput()->GetKey(Input::KEY_INFO::A_KEY) &&
				!mGame->GetInput()->GetKey(Input::KEY_INFO::D_KEY) &&
				!mGame->GetInput()->GetKey(Input::KEY_INFO::S_KEY)
				) {
				mDirection = XMINT2(-1, 1);
				mASC->SetId(Dir::BackLeft);
			}
			if (
				mGame->GetInput()->GetKey(Input::KEY_INFO::S_KEY) &&
				mGame->GetInput()->GetKey(Input::KEY_INFO::D_KEY) &&
				!mGame->GetInput()->GetKey(Input::KEY_INFO::W_KEY) &&
				!mGame->GetInput()->GetKey(Input::KEY_INFO::A_KEY)
				) {
				mDirection = XMINT2(1, -1);
				mASC->SetId(Dir::FrontLeft);
			}
			if (
				mGame->GetInput()->GetKey(Input::KEY_INFO::S_KEY) &&
				mGame->GetInput()->GetKey(Input::KEY_INFO::A_KEY) &&
				!mGame->GetInput()->GetKey(Input::KEY_INFO::W_KEY) &&
				!mGame->GetInput()->GetKey(Input::KEY_INFO::D_KEY)
				) {
				mDirection = XMINT2(-1, -1);
				mASC->SetId(Dir::FrontRight);
			}
		}
		// Shiftを離すと向いている方向に移動する
		if (mGame->GetInput()->GetKeyExit(Input::SHIFT_KEY)) {
			mState = Const::State::Move;
		}

		if (mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::Z_KEY)) {
			mGame->GetChrManager()->AttackRequest(mPosition, mDirection);
			// ターンを敵に渡す
			// アニメーションを付けるならAttackにする
			mState = Const::State::Idle;
			TurnEnd();
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
		TurnEnd();
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

void Hero::TurnEnd() {
	mGame->GetChrManager()->ChangePhase();
	mActCnt = (mActCnt + 1) % 1000;
	// 3ターンごとにHPを回復させる
	if (!(mActCnt % 3)) {
		if (mPC->GetHp() < mPC->GetMaxHp() && mPC->GetHunger() > 0) {
			mPC->SetHp((mPC->GetHp() + 1) % (mPC->GetMaxHp() + 1));
		}
	}
	// 5ターンごとに満腹度を減らす
	if (!(mActCnt % 5)) {
		if (mPC->GetHunger() > 0) {
			mPC->SetHunger((mPC->GetHunger() - 1) % (101));
		}
	}
	// 満腹度が0の状態で歩くとHPが減る
	if (mPC->GetHunger() == 0) {
		mPC->SetHp((mPC->GetHp() - 1));
	}
}