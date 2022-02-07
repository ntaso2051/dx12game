#include "Item.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "Hero.h"
#include "DungeonGenerator.h"
#include "ImguiWrapper.h"

Item::Item(Game* game, XMFLOAT3 pos, Const::TexId::Type texId) : Entity(game, pos), mInfo("") {
	mSpriteComponent = new SpriteComponent(this, texId);
	mGame->GetDgGen()->SetCellType(pos.x, pos.y, Const::Cell::Item);
	mUpdateOrder = 100;
}

Item::Item(Game* game) : Entity(game, XMFLOAT3(-1.0f, -1.0f, 0.0f)) {
	mUpdateOrder = 100;
}

Item::~Item() {
}

void Item::UpdateEntity(float deltaTime) {
	if (mSpriteComponent != nullptr) {
		if (mSpriteComponent->ActiveSelf()) {
			XMFLOAT3 heroPos = mGame->GetHero()->GetPosition();
			if (heroPos.x == mPosition.x && heroPos.y == mPosition.y) {
				mSpriteComponent->SetActive(false);
				mGame->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Hero);
				mGame->GetHero()->PushItem(this);
				mGame->RemoveFallenItem(this);
				std::string log = mName + u8"‚ð‚Ä‚É‚¢‚ê‚½I";
				mGame->GetImgui()->Cout(log);
			}
		}
	}
}

void Item::Adapt() {

}

void Item::Deadapt() {

}

void Item::Remove() {
	std::string log = mName + u8"‚ð‚·‚Ä‚½I";
	mGame->GetImgui()->Cout(log);
	mGame->GetHero()->RemoveItem(this);
}