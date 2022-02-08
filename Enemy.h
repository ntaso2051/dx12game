#pragma once
#include "Entity.h"
#include <string>
#include "Const.h"

class Enemy : public Entity {
public:
	Enemy(class Game* game, XMFLOAT3 pos);
	~Enemy();
	bool SearchEightDirections();
	void UpdateEntity(float deltaTime) override;
	void SetState(Const::State state) { mState = state; }
	Const::State GetState() { return mState; }
private:
	class SpriteComponent* mSc;
	XMINT2 mDirection;
	XMINT2 mPrePos;
	Const::State mState;
	bool mIsTarget;
};