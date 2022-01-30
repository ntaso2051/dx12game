#include "Hero.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "Const.h"
#include "Input.h"
#include "DungeonGenerator.h"

using namespace DirectX;

Hero::Hero(Game* game) :Entity(game) {
	SpriteComponent* sc = new SpriteComponent(this, Const::TexId::Hero);
	auto img = sc->GetSpriteRenderer()->GetTexture()->GetImgData();
	mPosition = XMINT2(0, 0);
	mGame->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Actor);
}

Hero::Hero(Game* game, XMFLOAT3 pos) :Entity(game) {
	SpriteComponent* sc = new SpriteComponent(this, Const::TexId::Hero);
	auto img = sc->GetSpriteRenderer()->GetTexture()->GetImgData();
	mPosition = XMINT2(pos.x, pos.y);
	mGame->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Actor);
}

void Hero::UpdateEntity(float deltaTime) {
	// 移動元をFloorに変更
	mGame->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Floor);
	if (
		mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::W_KEY) &&
		mGame->GetDgGen()->getCellType(mPosition.x, mPosition.y + 1) != Const::Cell::Wall
		) {
		mPosition.y++;
	}
	if (
		mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::S_KEY) &&
		mGame->GetDgGen()->getCellType(mPosition.x, mPosition.y - 1) != Const::Cell::Wall
		) {
		mPosition.y--;
	}
	if (
		mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::D_KEY) &&
		mGame->GetDgGen()->getCellType(mPosition.x + 1, mPosition.y) != Const::Cell::Wall
		) {
		mPosition.x++;
	}
	if (
		mGame->GetInput()->GetKeyEnter(Input::KEY_INFO::A_KEY) &&
		mGame->GetDgGen()->getCellType(mPosition.x - 1, mPosition.y) != Const::Cell::Wall
		) {
		mPosition.x--;
	}
	// 移動先をActorに変更
	mGame->GetDgGen()->SetCellType(mPosition.x, mPosition.y, Const::Cell::Actor);
	// とりあえずRotateする
	// mWorldMat *= XMMatrixRotationZ(deltaTime);
}

