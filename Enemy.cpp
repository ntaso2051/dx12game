#include "Enemy.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "ParameterComponent.h"
#include "Const.h"
#include "DungeonGenerator.h"
#include "CharacterManager.h"
#include "Hero.h"
#include "Random.h"

Enemy::Enemy(Game* game, XMFLOAT3 pos) : Entity(game, pos), mDirection(XMINT2(0, -1)), mPrePos(XMINT2(pos.x, pos.y)), mState(Const::State::Idle) {
	mSc = new SpriteComponent(this, Const::TexId::Blob);
	ParameterComponent* pc = new ParameterComponent(this, Const::BLOB_INIT_HP, Const::BLOB_INIT_EXP, Const::BLOB_INIT_LEVEL, Const::BLOB_INIT_ATTACK);
	pc->SetAttack(mGame->GetFloorNum() * 2 + pc->GetAttack());
	pc->SetMaxHp(mGame->GetFloorNum() * 2 + pc->GetMaxHp());
	pc->SetHp(mGame->GetFloorNum() * 2 + pc->GetHp());
	pc->SetExp(mGame->GetFloorNum() + pc->GetExp());
	mGame->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Enemy);
	mUpdateOrder = 110;
}

Enemy::~Enemy() {

}


bool Enemy::SearchEightDirections() {
	auto e = mGame->GetDgGen()->getCellType(mPrePos.x + 1, mPrePos.y);
	auto w = mGame->GetDgGen()->getCellType(mPrePos.x - 1, mPrePos.y);
	auto n = mGame->GetDgGen()->getCellType(mPrePos.x, mPrePos.y + 1);
	auto s = mGame->GetDgGen()->getCellType(mPrePos.x, mPrePos.y - 1);
	auto ne = mGame->GetDgGen()->getCellType(mPrePos.x + 1, mPrePos.y + 1);
	auto nw = mGame->GetDgGen()->getCellType(mPrePos.x - 1, mPrePos.y + 1);
	auto sw = mGame->GetDgGen()->getCellType(mPrePos.x - 1, mPrePos.y - 1);
	auto se = mGame->GetDgGen()->getCellType(mPrePos.x + 1, mPrePos.y - 1);

	if (e == Const::Cell::Hero || w == Const::Cell::Hero || n == Const::Cell::Hero || s == Const::Cell::Hero || ne == Const::Cell::Hero || nw == Const::Cell::Hero || sw == Const::Cell::Hero || se == Const::Cell::Hero) {
		return true;
	}

	return false;
}

void Enemy::UpdateEntity(float deltaTime) {
	if (mGame->GetChrManager()->GetPhase() == CharacterManager::Phase::EnemiesPhase) {
		if (mState == Const::State::Idle) {
			if (SearchEightDirections()) {	// 攻撃する
				mGame->GetChrManager()->AttackRequestByEnemy(this->GetComponent("ParameterComponent"));
				// Attackアニメーションを作るときにAttackを使う
				mState = Const::State::End;
				mGame->GetChrManager()->IncEnemyCnt();
			}
			else {	// 攻撃しない
				mGame->GetDgGen()->SetCellType(mPrePos.x, mPrePos.y, Const::Cell::Floor);
				XMFLOAT3 heroPos = mGame->GetHero()->GetPosition();
				int heroRoomId = mGame->GetDgGen()->GetCurrentRoom(XMINT2(heroPos.x, heroPos.y));
				int myRoomId = mGame->GetDgGen()->GetCurrentRoom(XMINT2(mPosition.x, mPosition.y));
				if (heroRoomId == myRoomId) {
					mDirection.x = (heroPos.x - mPosition.x >= 0) ? 1 : -1;
					mDirection.y = (heroPos.y - mPosition.y >= 0) ? 1 : -1;
				}
				else {
					mDirection.x = 1;
					mDirection.y = 0;
				}
				int x = mPrePos.x + mDirection.x;
				int y = mPrePos.y + mDirection.y;
				auto cellType = mGame->GetDgGen()->getCellType(x, y);
				if (
					cellType != Const::Cell::Wall &&
					cellType != Const::Cell::Enemy &&
					cellType != Const::Cell::Hero
					) {
					mState = Const::State::Move;
					mGame->GetDgGen()->SetCellType(x, y, Const::Cell::Enemy);
				}
				else {
					mGame->GetDgGen()->SetCellType(mPrePos.x, mPrePos.y, Const::Cell::Enemy);
					mState = Const::State::End;
					mGame->GetChrManager()->IncEnemyCnt();
				}
			}
		}
		else if (mState == Const::State::Move) {
			mPosition.x += deltaTime * 5 * mDirection.x;
			mPosition.y += deltaTime * 5 * mDirection.y;

			if (std::abs(mPosition.x - mPrePos.x) >= 0.999f || std::abs(mPosition.y - mPrePos.y) >= 0.999f) {
				mPosition.x = mPrePos.x + mDirection.x;
				mPosition.y = mPrePos.y + mDirection.y;
				mState = Const::State::End;
				mPrePos = XMINT2(mPosition.x, mPosition.y);
				mGame->GetChrManager()->IncEnemyCnt();
			}
		}
	}
}