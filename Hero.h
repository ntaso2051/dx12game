#pragma once
#include "Entity.h"

class Hero :public Entity {
public:
	Hero(class Game* game);
	void UpdateEntity(float deltaTime) override;
};