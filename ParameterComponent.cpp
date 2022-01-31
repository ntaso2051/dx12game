#include "ParameterComponent.h"
#include "Entity.h"

ParameterComponent::ParameterComponent(Entity* owner, int hp, int exp, int level, int attack) : Component(owner), mHp(hp), mExp(exp), mLevel(level), mAttack(attack) {

}

ParameterComponent::~ParameterComponent() {}

void ParameterComponent::Damaged(ParameterComponent* pc) {
	mHp -= pc->mAttack;
	mExp += pc->mExp;
}