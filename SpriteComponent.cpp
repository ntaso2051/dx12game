#include "SpriteComponent.h"
#include "SpriteRenderer.h"
#include "Entity.h"
#include "Game.h"
#include "Window.h"

SpriteComponent::SpriteComponent(Entity* owner, int drawOrder) :Component(owner), mDrawOrder(drawOrder) {
	mOwner->AddComponent(this);
	// init SpriteRenderer
	mSpriteRenderer = new SpriteRenderer(mOwner->GetGame()->GetDx12());
	Window* window = mOwner->GetGame()->GetWindow();
	mSpriteRenderer->InitView(window->GetWidth(), window->GetHeight());
	mSpriteRenderer->CompileShader();
	mSpriteRenderer->InitRootSignature();
	mSpriteRenderer->InitGraphicPipeline();
	mSpriteRenderer->CreateTexture(window->GetWidth(), window->GetHeight());
}

SpriteComponent::~SpriteComponent() {
	mOwner->RemoveComponent(this);
}

void SpriteComponent::Draw() {
	mSpriteRenderer->Draw();
}

void SpriteComponent::Update(float deltaTime) {
	mSpriteRenderer->Draw();
}