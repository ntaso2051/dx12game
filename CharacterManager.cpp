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

CharacterManager::CharacterManager(Hero* hero) : mHero(hero), mEnemiesCount(0) {
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
}

void CharacterManager::ChangePhase() {
	if (mPhase == Phase::HeroPhase) {
		mPhase = Phase::EnemiesPhase;
	}
	else {
		mPhase = Phase::HeroPhase;
		for (auto e : mEnemies) {
			e->SetEndTurn();
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
	if (enemy != nullptr) {
		DamageCalc(static_cast<ParameterComponent*>(mHero->GetComponent("ParameterComponent")), static_cast<ParameterComponent*>(static_cast<Enemy*>(enemy)->GetComponent("ParameterComponent")));
	}
}


void CharacterManager::DamageCalc(ParameterComponent* attack, ParameterComponent* damaged) {
	damaged->Damaged(attack);
}

void CharacterManager::AddEnemy(Enemy* enemy) {
	mEnemies.push_back(enemy);
}

void CharacterManager::EraseEnemy(Enemy* en) {
	for (auto it = mEnemies.begin(); it != mEnemies.end();) {
		if (*it == en) {
			it = mEnemies.erase(it);
			delete en;
		}
		else {
			++it;
		}
	}
}


// éÄñSí ímÇPrameterComponentÇ©ÇÁëóÇÈÇÃÇ≈ÅCEntity->EnemyÇÃïœä∑ÇÇ±Ç±Ç≈Ç‚ÇÈÅD
void CharacterManager::RemoveEnemy(Entity* enemy) {
	// if is in entities
	Enemy* en = static_cast<Enemy*>(enemy);
	XMFLOAT3 pos = en->GetPosition();
	en->GetGame()->GetDgGen()->SetCellType(pos.x, pos.y, Const::Cell::Floor);
	static_cast<SpriteComponent*>(en->GetComponent("SpriteComponent"))->SetActive(false);
}