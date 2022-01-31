#include "CharacterManager.h"
#include "Entity.h"
#include "EnemyBlob.h"
#include "Hero.h"
#include "ParameterComponent.h"
#include <algorithm>

CharacterManager::CharacterManager(Hero* hero) : mHero(hero) {
	mPhase = Phase::HeroPhase;
}

CharacterManager::~CharacterManager() {
	delete mHero;
	for (auto e : mEnemyBlobs) {
		delete e;
	}
}

void CharacterManager::ChangePhase() {
	if (mPhase == Phase::HeroPhase) {
		mPhase = Phase::EnemiesPhase;
	}
	else {
		mPhase = Phase::HeroPhase;
	}
}

Entity* CharacterManager::FindEnemyByPos(XMINT2 pos) {
	for (auto e : mEnemyBlobs) {
		if (e->GetPosition().x == pos.x && e->GetPosition().y == pos.y) {
			return static_cast<Entity*>(e);
		}
	}
	return nullptr;
}

void CharacterManager::AttackRequest(XMINT2 pos, XMINT2 dir) {
	Entity* enemy = FindEnemyByPos(XMINT2(pos.x + dir.x, pos.y + dir.y));
	if (enemy != nullptr) {
		DamageCalc(static_cast<ParameterComponent*>(mHero->GetHeroParam()), static_cast<ParameterComponent*>(static_cast<EnemyBlob*>(enemy)->GetParam()));
	}
}


void CharacterManager::DamageCalc(ParameterComponent* attack, ParameterComponent* damaged) {
	damaged->Damaged(attack);
}

void CharacterManager::AddEnemyBlob(EnemyBlob* enemyBlob) {
	mEnemyBlobs.push_back(enemyBlob);
}

void CharacterManager::RemoveEnemyBlob(EnemyBlob* enemyBlob) {
	// if is in entities
	auto iter = std::find(mEnemyBlobs.begin(), mEnemyBlobs.end(), enemyBlob);
	if (iter != mEnemyBlobs.end()) {
		std::iter_swap(iter, mEnemyBlobs.end() - 1);
		mEnemyBlobs.pop_back();
	}
}