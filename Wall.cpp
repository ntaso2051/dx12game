#include "Wall.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "SpriteRenderer.h"
#include "Texture.h"

Wall::Wall(Game* game) :Entity(game) {
	SpriteComponent* sc = new SpriteComponent(this, L"Resources/Images/Wall.png");
	auto img = sc->GetSpriteRenderer()->GetTexture()->GetImgData();
	mWorldMat = XMMatrixScaling(1.0f / img->width, 1.0f / img->height, 1.0f);
	mWorldMat *= XMMatrixTranslation(2.0f / img->width, 0.0f, 0.0f);
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

Wall::Wall(Game * game, XMFLOAT3 pos) :Entity(game) {
	SpriteComponent* sc = new SpriteComponent(this, L"Resources/Images/Wall.png");
	auto img = sc->GetSpriteRenderer()->GetTexture()->GetImgData();
	mWorldMat = XMMatrixScaling(1.0f / img->width, 1.0f / img->height, 1.0f);
	mWorldMat *= XMMatrixTranslation(pos.x * 2 / img->width, pos.y * 2 / img->height, 0.0f);
	XMFLOAT3 eye(0, 0, -5);
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

Wall::~Wall() {}