#include "CharacterManager.h"
#include "Entity.h"
#include "Enemy.h"
#include "Hero.h"
#include "ParameterComponent.h"
#include <algorithm>
#include <typeinfo>
#include "Game.h"
#include "DungeonGenerator.h"
#include "SpriteComponent.h"
#include <string>
#include "ImguiWrapper.h"

CharacterManager::CharacterManager(Hero* hero) : mHero(hero), mEnemiesCount(0), mEnemiesTotal(Const::INIT_ENEMIES_NUM) {
	mPhase = Phase::HeroPhase;
}

CharacterManager::~CharacterManager() {
	for (auto e : mEnemies) {
		delete e;
	}
	mEnemies.clear();
}

void CharacterManager::Update() {
	if (mEnemiesCount == mEnemies.size()) {
		ChangePhase();
		mEnemiesCount = 0;
	}

	for (auto de : mDeadEnemies) {
		// delete de;
	}
	if (mEnemies.size() == 0) {
		mHero->GetGame()->CreateEnemies();
	}
}

void CharacterManager::ChangePhase() {
	if (mEnemies.size()) {
		if (mPhase == Phase::HeroPhase) {
			mPhase = Phase::EnemiesPhase;
		}
		else {
			mPhase = Phase::HeroPhase;
			for (auto e : mEnemies) {
				if (e->GetState() != Const::State::Dead) {
					e->SetState(Const::State::Idle);
				}
			}
		}
	}
}

Entity* CharacterManager::FindEnemyByPos(XMINT2 pos) {
	for (auto e : mEnemies) {
		if (e->GetPosition().x == pos.x && e->GetPosition().y == pos.y) {
			return static_cast<Entity*>(e);
		}
	}
	return nullptr;
}

void CharacterManager::AttackRequest(XMFLOAT3 pos, XMINT2 dir) {
	Entity* enemy = FindEnemyByPos(XMINT2(pos.x + dir.x, pos.y + dir.y));
	ParameterComponent * heroParam = static_cast<ParameterComponent*>(mHero->GetComponent("ParameterComponent"));
	if (enemy != nullptr) {
		DamageCalc(heroParam, static_cast<ParameterComponent*>(static_cast<Enemy*>(enemy)->GetComponent("ParameterComponent")));
		mHero->GetGame()->GetImgui()->Cout(u8"プレイヤーのこうげき！");
		std::string log = u8"てきに" + std::to_string(heroParam->GetAttack()) + u8"のダメージをあたえた！";
		mHero->GetGame()->GetImgui()->Cout(log);
	}
}

void CharacterManager::AttackRequestByEnemy(Component* param) {
	DamageCalc(static_cast<ParameterComponent*>(param), static_cast<ParameterComponent*>(mHero->GetComponent("ParameterComponent")));
}

void CharacterManager::DamageCalc(ParameterComponent* attack, ParameterComponent* damaged) {
	if (attack != nullptr && damaged != nullptr)
		damaged->Damaged(attack);
}

void CharacterManager::AddEnemy(Enemy * enemy) {
	mEnemies.push_back(enemy);
}

void CharacterManager::EraseEnemy(Enemy * en) {
	for (auto it = mEnemies.begin(); it != mEnemies.end();) {
		if (*it == en) {
			it = mEnemies.erase(it);
			// delete en;
		}
		else {
			++it;
		}
	}
}


// 死亡通知をPrameterComponentから送るので，Entity->Enemyの変換をここでやる．
void CharacterManager::RemoveEnemy(Entity * enemy) {
	Enemy* en = static_cast<Enemy*>(enemy);
	XMFLOAT3 pos = en->GetPosition();
	// 元居た場所をFloorにする．
	en->GetGame()->GetDgGen()->SetCellType(pos.x, pos.y, Const::Cell::Floor);
	// 描画しなくする．
	static_cast<SpriteComponent*>(en->GetComponent("SpriteComponent"))->SetActive(false);
	// StateをDeadにする．
	en->SetState(Const::State::Dead);
	mEnemiesTotal--;
	en->SetPosition(XMFLOAT3(-1, -1, 0));
	EraseEnemy(en);
	mDeadEnemies.emplace_back(en);
}