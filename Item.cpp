#include "Item.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "Hero.h"
#include "DungeonGenerator.h"

Item::Item(Game* game, XMFLOAT3 pos, Const::TexId::Type texId) : Entity(game, pos) {
	mSpriteComponent = new SpriteComponent(this, texId);
	mGame->GetDgGen()->SetCellType(pos.x, pos.y, Const::Cell::Item);
	mUpdateOrder = 100;
}

Item::~Item() {
}

void Item::UpdateEntity(float deltaTime) {
	if (mSpriteComponent->ActiveSelf()) {
		XMFLOAT3 heroPos = mGame->GetHero()->GetPosition();
		if (heroPos.x == mPosition.x && heroPos.y == mPosition.y) {
			mSpriteComponent->SetActive(false);
			mGame->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Hero);
		}
	}
}