#include "Hero.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "Const.h"

using namespace DirectX;

Hero::Hero(Game* game) :Entity(game) {
	SpriteComponent* sc = new SpriteComponent(this, Const::TexId::Hero);
	auto img = sc->GetSpriteRenderer()->GetTexture()->GetImgData();
	mWorldMat = XMMatrixScaling(1.0f / img->width, 1.0f / img->height, 1.0f);
	XMFLOAT3 eye(0, 0, -1);
	XMFLOAT3 target(0, 0, 0);
	XMFLOAT3 up(0, 1, 0);
	mViewMat = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	mProjMat = XMMatrixPerspectiveFovLH(
		XM_PIDIV4,
		static_cast<double>(GetGame()->GetWindow()->GetWidth()) / static_cast<double>(GetGame()->GetWindow()->GetHeight()),
		1.0f,//‹ß‚¢•û
		10.0f//‰“‚¢•û
	);
}

Hero::Hero(Game * game, XMFLOAT3 pos) :Entity(game) {
	SpriteComponent* sc = new SpriteComponent(this, Const::TexId::Hero);
	auto img = sc->GetSpriteRenderer()->GetTexture()->GetImgData();
	mWorldMat = XMMatrixScaling(1.0f / img->width, 1.0f / img->height, 1.0f);
	mWorldMat *= XMMatrixTranslation(pos.x * 2 / img->width, pos.y * 2 / img->height, 0.0f);
	XMFLOAT3 eye(0, 0, -1);
	XMFLOAT3 target(0, 0, 0);
	XMFLOAT3 up(0, 1, 0);
	mViewMat = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	mProjMat = XMMatrixPerspectiveFovLH(
		XM_PIDIV4,
		static_cast<double>(GetGame()->GetWindow()->GetWidth()) / static_cast<double>(GetGame()->GetWindow()->GetHeight()),
		1.0f,//‹ß‚¢•û
		10.0f//‰“‚¢•û
	);
}

void Hero::UpdateEntity(float deltaTime) {
	// ‚Æ‚è‚ ‚¦‚¸Rotate‚·‚é
	mWorldMat *= XMMatrixRotationZ(deltaTime);
}

