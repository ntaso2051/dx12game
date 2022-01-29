#include "Hero.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "Const.h"
#include "Input.h"

using namespace DirectX;

Hero::Hero(Game* game) :Entity(game) {
	SpriteComponent* sc = new SpriteComponent(this, Const::TexId::Hero);
	auto img = sc->GetSpriteRenderer()->GetTexture()->GetImgData();
	mWorldMat = XMMatrixScaling(1.0f / img->width, 1.0f / img->height, 1.0f);
}

Hero::Hero(Game* game, XMFLOAT3 pos) :Entity(game) {
	SpriteComponent* sc = new SpriteComponent(this, Const::TexId::Hero);
	auto img = sc->GetSpriteRenderer()->GetTexture()->GetImgData();
	mWorldMat = XMMatrixScaling(1.0f / img->width, 1.0f / img->height, 1.0f);
	mWorldMat *= XMMatrixTranslation(pos.x * 2 / img->width, pos.y * 2 / img->height, 0.0f);

}

void Hero::UpdateEntity(float deltaTime) {
	if (mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::W_KEY)) {
		mWorldMat *= XMMatrixTranslation(0.0f, 2.0f / Const::CELL_SIZE, 0.0f);
	}
	if (mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::S_KEY)) {
		mWorldMat *= XMMatrixTranslation(0.0f, -2.0f / Const::CELL_SIZE, 0.0f);
	}
	if (mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::D_KEY)) {
		mWorldMat *= XMMatrixTranslation(2.0f / Const::CELL_SIZE, 0.0f, 0.0f);
	}
	if (mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::A_KEY)) {
		mWorldMat *= XMMatrixTranslation(-2.0f / Const::CELL_SIZE, 0.0f, 0.0f);
	}
	// ‚Æ‚è‚ ‚¦‚¸Rotate‚·‚é
	// mWorldMat *= XMMatrixRotationZ(deltaTime);
}

