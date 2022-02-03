#include "SpriteComponent.h"
#include "SpriteRenderer.h"
#include "Entity.h"
#include "Game.h"
#include "Window.h"
#include "Hero.h"
#include "Const.h"

SpriteComponent::SpriteComponent(Entity* owner, int textureId, int drawOrder) :Component(owner), mDrawOrder(drawOrder), mActiveSelf(true) {
	// init SpriteRenderer
	mSpriteRenderer = new SpriteRenderer(mOwner->GetGame()->GetDx12(), owner->GetGame()->GetTextureById(textureId));
	Window* window = mOwner->GetGame()->GetWindow();
	// mSpriteRenderer->InitView(window->GetWidth(), window->GetHeight(), filename);
	// mSpriteRenderer->CompileShader();
	// mSpriteRenderer->InitRootSignature();
	// mSpriteRenderer->InitGraphicPipeline();
	mSpriteRenderer->CreateTexture(window->GetWidth(), window->GetHeight());

}

SpriteComponent::~SpriteComponent() {
	delete mSpriteRenderer;
	mOwner->RemoveComponent(this);
}

void SpriteComponent::Draw() {
	if (mActiveSelf)
		mSpriteRenderer->Draw();
}

void SpriteComponent::SetActive(bool active) {
	mActiveSelf = active;
}

void SpriteComponent::Update(float deltaTime) {
	if (mActiveSelf) {
		XMMATRIX worldMat = XMMatrixScaling(1.0f / Const::CELL_SIZE, 1.0f / Const::CELL_SIZE, 1.0f);
		XMFLOAT3 heroPos = mOwner->GetGame()->GetHero()->GetPosition();
		XMFLOAT3 ownerPos = mOwner->GetPosition();
		worldMat *= XMMatrixTranslation(2.0f * (ownerPos.x - heroPos.x) / Const::CELL_SIZE, 2.0f * (ownerPos.y - heroPos.y) / Const::CELL_SIZE, 1.0f);
		mSpriteRenderer->ReComputeMatrix(worldMat, mOwner->GetGame()->GetViewMat(), mOwner->GetGame()->GetProjMat());
		mSpriteRenderer->Draw();
	}
}