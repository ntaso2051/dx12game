#include "ParameterComponent.h"
#include "Entity.h"

#include "Game.h"
#include "DungeonGenerator.h"
#include "CharacterManager.h"
#include "EnemyBlob.h"
#include "SpriteComponent.h"

ParameterComponent::ParameterComponent(Entity* owner, int hp, int exp, int level, int attack) : Component(owner), mHp(hp), mExp(exp), mLevel(level), mAttack(attack) {

}

ParameterComponent::~ParameterComponent() {}

void ParameterComponent::Damaged(ParameterComponent* pc) {
	mHp -= pc->mAttack;
	pc->mExp += mExp;
	if (mHp < 0) {
		XMINT2 pos = mOwner->GetPosition();
		mOwner->GetGame()->GetDgGen()->SetCellType(pos.x, pos.y, Const::Cell::Floor);
		mOwner->GetGame()->GetChrManager()->RemoveEnemyBlob(static_cast<EnemyBlob*>(mOwner));
		static_cast<SpriteComponent*> (static_cast<EnemyBlob*>(mOwner)->GetSpriteComponent())->SetActive(false);
	}
}