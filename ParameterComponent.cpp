#include "ParameterComponent.h"
#include "Entity.h"

#include "Game.h"
#include "DungeonGenerator.h"
#include "CharacterManager.h"
#include "Enemy.h"
#include "SpriteComponent.h"

ParameterComponent::ParameterComponent(Entity* owner, int hp, int exp, int level, int attack) : Component(owner), mHp(hp), mExp(exp), mLevel(level), mAttack(attack) {

}

ParameterComponent::~ParameterComponent() {}

void ParameterComponent::Damaged(ParameterComponent* pc) {
	mHp -= pc->mAttack;
	if (mHp < 0) {
		pc->mExp += mExp;
		mOwner->GetGame()->GetChrManager()->RemoveEnemy(mOwner);
	}
}