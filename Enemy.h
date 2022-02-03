#pragma once
#include "Entity.h"
#include <string>

class Enemy : public Entity {
public:
	Enemy(class Game* game, XMFLOAT3 pos);
	~Enemy();
	void UpdateEntity(float deltaTime) override;
	void SetEndTurn() { mEndTurn = false; }
private:
	class SpriteComponent* mSc;
	XMINT2 mDirection;
	XMINT2 mPrePos;
	bool mIsMoving;
	bool mEndTurn;
};