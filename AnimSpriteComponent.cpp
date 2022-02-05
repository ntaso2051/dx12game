#include "AnimSpriteComponent.h"
#include "Entity.h"
#include "SpriteComponent.h"
#include "SpriteRenderer.h"
#include "Dx12Wrapper.h"
#include "Game.h"
#include "Const.h"

AnimSpriteComponent::AnimSpriteComponent(Entity* owner, std::vector<std::vector<int>>texIdss, int drawOrder) :Component(owner, drawOrder), frame(0.0f), mId(0) {
	for (auto texIds : texIdss) {
		std::vector<SpriteRenderer*> srs;
		for (auto texId : texIds) {
			SpriteRenderer* sr = new SpriteRenderer(mOwner->GetGame()->GetDx12(), mOwner->GetGame()->GetTextureById(texId));
			sr->CreateTexture(Const::WINDOW_WIDTH, Const::WINDOW_HEIGHT);
			srs.push_back(sr);
		}
		mSpriteRenderers.push_back(srs);
	}
	mSpriteComponent = static_cast<SpriteComponent*>(mOwner->GetComponent("SpriteComponent"));
}

AnimSpriteComponent::~AnimSpriteComponent() {
	for (auto row : mSpriteRenderers) {
		for (auto sr : row) {
			delete sr;
		}
	}
}

void AnimSpriteComponent::Update(float deltaTime) {
	if (mSpriteComponent->ActiveSelf()) {
		frame += deltaTime * 4;
		mSpriteComponent->SetSpriteRenderer(mSpriteRenderers[mId][int(frame) % mSpriteRenderers[mId].size()]);
	}
}
