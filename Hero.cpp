#include "Hero.h"
#include "Game.h"
#include "SpriteComponent.h"

Hero::Hero(Game* game) :Entity(game) {
	SpriteComponent* sc = new SpriteComponent(this);
}

void Hero::UpdateEntity(float deltaTime) {

}

