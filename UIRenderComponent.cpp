#include "UIRenderComponent.h"
#include "Entity.h"
#include "Game.h"
#include "SpriteRenderer.h"

UIRenderComponent::UIRenderComponent(Entity* owner, int texId, float w, float h, float x, float y) :Component(owner), mActiveSelf(true), mWidth(w), mHeight(h), mPosX(x), mPosY(y) {
	mSpriteRenderer = new SpriteRenderer(mOwner->GetGame()->GetDx12(), mOwner->GetGame()->GetTextureById(texId));
	Window* win = mOwner->GetGame()->GetWindow();
	mSpriteRenderer->CreateTexture(win->GetWidth(), win->GetHeight());
}

UIRenderComponent::~UIRenderComponent() {
	delete mSpriteRenderer;
	mOwner->RemoveComponent(this);
}


void UIRenderComponent::Update(float deltaTime) {
	if (mActiveSelf) {
		XMMATRIX worldMat = XMMatrixScaling(1.0f / mWidth, 1.0f / mHeight, 1.0f);
		worldMat *= XMMatrixTranslation(mPosX / mWidth, mPosY / mHeight, 0.0f);
		mSpriteRenderer->ReComputeMatrix(worldMat, mOwner->GetGame()->GetViewMat(), mOwner->GetGame()->GetProjMat());
		mSpriteRenderer->Draw();
	}
}

void UIRenderComponent::Draw() {
	if (mActiveSelf) {
		mSpriteRenderer->Draw();
	}
}

