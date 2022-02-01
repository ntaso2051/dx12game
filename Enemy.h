#pragma once
#include "Entity.h"
#include <string>

class Enemy : public Entity {
public:
	Enemy(class Game* game, XMFLOAT3 pos);
	~Enemy();
	void UpdateEntity(float deltaTime) override;
private:
};