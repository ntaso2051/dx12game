#pragma once
#include "Entity.h"

class Hero :public Entity {
public:
	Hero(class Game* game);
	void UpdateEntity(float deltaTime) override;
private:
	// �Ƃ肠����
	float move = 0.0f;
};