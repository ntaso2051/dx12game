#include "AnimSpriteComponent.h"
#include "Entity.h"
#include "SpriteComponent.h"
#include "SpriteRenderer.h"
#include "Dx12Wrapper.h"
#include "Game.h"
#include "Const.h"

AnimSpriteComponent::AnimSpriteComponent(Entity* owner, std::vector<int>texIds, int drawOrder) :Component(owner, drawOrder), frame(0.0f) {
	for (auto texId : texIds) {
		SpriteRenderer* sr = new SpriteRenderer(mOwner->GetGame()->GetDx12(), mOwner->GetGame()->GetTextureById(texId));
		sr->CreateTexture(Const::WINDOW_WIDTH, Const::WINDOW_HEIGHT);
		mSpriteRenderers.push_back(sr);
	}
	mSpriteComponent = static_cast<SpriteComponent*>(mOwner->GetComponent("SpriteComponent"));
}

AnimSpriteComponent::~AnimSpriteComponent() {
	for (auto sr : mSpriteRenderers) {
		delete sr;
	}
}

void AnimSpriteComponent::Update(float deltaTime) {
	if (mSpriteComponent->ActiveSelf()) {
		frame += deltaTime * 4;
		mSpriteComponent->SetSpriteRenderer(mSpriteRenderers[int(frame) % mSpriteRenderers.size()]);
	}
}