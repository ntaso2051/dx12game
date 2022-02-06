#include "ParameterComponent.h"
#include "Entity.h"

#include "Game.h"
#include "DungeonGenerator.h"
#include "CharacterManager.h"
#include "Enemy.h"
#include "SpriteComponent.h"
#include "ImguiWrapper.h"

ParameterComponent::ParameterComponent(Entity* owner, int hp, int exp, int level, int attack) : Component(owner), mMaxHp(hp), mHp(hp), mExp(exp), mLevel(level), mAttack(attack), mHunger(100) {
	UpdateStatus();
}

ParameterComponent::~ParameterComponent() {}

void ParameterComponent::Damaged(ParameterComponent* pc) {
	mHp -= pc->mAttack;
	if (mHp < 0) {
		pc->mExp += mExp;
		mOwner->GetGame()->GetChrManager()->RemoveEnemy(mOwner);
	}
}

void ParameterComponent::LevelUp() {
	if (mExp >= (1 << (mLevel + 1))) {
		mLevel++;
		std::string log = u8"レベルが" + std::to_string(mLevel) + u8"にあがった！";
		mOwner->GetGame()->GetImgui()->Cout(log);
		UpdateStatus();
	}
}

void ParameterComponent::UpdateStatus() {
	if (!(mLevel % 2)) {
		mAttack++;
		mMaxHp += 2;
	}
}

void ParameterComponent::Update(float deltaTime) {
	LevelUp();
}