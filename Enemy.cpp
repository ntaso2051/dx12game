#include "Enemy.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "ParameterComponent.h"
#include "Const.h"
#include "DungeonGenerator.h"
#include "CharacterManager.h"
#include "Hero.h"
#include "Random.h"

Enemy::Enemy(Game* game, XMFLOAT3 pos) : Entity(game, pos), mDirection(XMINT2(0, -1)), mPrePos(XMINT2(pos.x, pos.y)), mIsMoving(false), mEndTurn(false) {
	mSc = new SpriteComponent(this, Const::TexId::Blob);
	ParameterComponent* pc = new ParameterComponent(this, Const::BLOB_INIT_HP, Const::BLOB_INIT_EXP, Const::BLOB_INIT_LEVEL, Const::BLOB_INIT_ATTACK);
	mGame->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Enemy);
	mUpdateOrder = 110;
}

Enemy::~Enemy() {
}


void Enemy::UpdateEntity(float deltaTime) {
	if (mGame->GetChrManager()->GetPhase() == CharacterManager::Phase::EnemiesPhase) {
		if (mSc->ActiveSelf()) {
			if (!mIsMoving && !mEndTurn) {
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
					mIsMoving = true;
					mGame->GetDgGen()->SetCellType(x, y, Const::Cell::Enemy);
				}
				else {
					mGame->GetDgGen()->SetCellType(mPrePos.x, mPrePos.y, Const::Cell::Enemy);
					mEndTurn = true;
					mGame->GetChrManager()->IncEnemyCnt();
				}
			}
			else if (mIsMoving && !mEndTurn) {
				mPosition.x += deltaTime * 5 * mDirection.x;
				mPosition.y += deltaTime * 5 * mDirection.y;

				if (std::abs(mPosition.x - mPrePos.x) >= 0.999f || std::abs(mPosition.y - mPrePos.y) >= 0.999f) {
					mPosition.x = mPrePos.x + mDirection.x;
					mPosition.y = mPrePos.y + mDirection.y;
					mIsMoving = false;
					mEndTurn = true;
					mPrePos = XMINT2(mPosition.x, mPosition.y);
					mGame->GetChrManager()->IncEnemyCnt();
				}
			}
		}
		else {
			if (!mEndTurn) {
				mGame->GetChrManager()->IncEnemyCnt();
				mEndTurn = true;
			}
		}
	}
}