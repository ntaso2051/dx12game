#pragma once
#include "Entity.h"
#include <DirectXMath.h>

class Hero :public Entity {
public:
	Hero(class Game* game);
	Hero(class Game* game, XMFLOAT3 pos);
	void UpdateEntity(float deltaTime) override;
private:
	// ‚Æ‚è‚ ‚¦‚¸
	float move = 0.0f;
};