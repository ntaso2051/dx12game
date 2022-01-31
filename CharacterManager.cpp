#include "CharacterManager.h"
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