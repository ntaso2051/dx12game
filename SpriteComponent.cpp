#include "SpriteComponent.h"
#include "SpriteRenderer.h"
#include "Entity.h"
#include "Game.h"
#include "Window.h"
#include "Hero.h"
#include "Const.h"

SpriteComponent::SpriteComponent(Entity* owner, int textureId, int drawOrder) :Component(owner), mDrawOrder(drawOrder) {
	mOwner->AddComponent(this);
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
	mOwner->RemoveComponent(this);
}

void SpriteComponent::Draw() {
	mSpriteRenderer->Draw();
}

void SpriteComponent::Update(float deltaTime) {
	XMMATRIX worldMat = XMMatrixScaling(1.0f / Const::CELL_SIZE, 1.0f / Const::CELL_SIZE, 1.0f);
	XMINT2 heroPos = mOwner->GetGame()->GetHero()->GetPosition();
	XMINT2 ownerPos = mOwner->GetPosition();
	worldMat *= XMMatrixTranslation(2.0f * (ownerPos.x - heroPos.x) / Const::CELL_SIZE, 2.0f * (ownerPos.y - heroPos.y) / Const::CELL_SIZE, 1.0f);
	mSpriteRenderer->ReComputeMatrix(worldMat, mOwner->GetGame()->GetViewMat(), mOwner->GetGame()->GetProjMat());
	mSpriteRenderer->Draw();
}