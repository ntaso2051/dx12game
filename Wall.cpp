#include "Wall.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "Const.h"

Wall::Wall(Game* game) :Entity(game) {
	SpriteComponent* sc = new SpriteComponent(this, Const::TexId::Wall);
	auto img = sc->GetSpriteRenderer()->GetTexture()->GetImgData();
	mWorldMat = XMMatrixScaling(1.0f / img->width, 1.0f / img->height, 1.0f);
	mWorldMat *= XMMatrixTranslation(2.0f / img->width, 0.0f, 0.0f);
}

Wall::Wall(Game* game, XMFLOAT3 pos) :Entity(game) {
	SpriteComponent* sc = new SpriteComponent(this, Const::TexId::Wall);
	auto img = sc->GetSpriteRenderer()->GetTexture()->GetImgData();
	mWorldMat = XMMatrixScaling(1.0f / img->width, 1.0f / img->height, 1.0f);
	mWorldMat *= XMMatrixTranslation(pos.x * 2 / img->width, pos.y * 2 / img->height, 0.0f);
}

Wall::~Wall() {}