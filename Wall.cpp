#include "Wall.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "Const.h"

Wall::Wall(Game* game, XMFLOAT3 pos) : Entity(game) {
	SpriteComponent* sc = new SpriteComponent(this, Const::TexId::Wall);
	mPosition = XMINT2(pos.x, pos.y);
}

Wall::~Wall() {

}