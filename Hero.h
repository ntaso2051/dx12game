#pragma once
#include "Entity.h"
#include <DirectXMath.h>
#include "Const.h"

class Hero :public Entity {
public:
	Hero(class Game* game, XMFLOAT3 pos);
	Hero(class Game* game, XMFLOAT3 pos, class ParameterComponent* param);
	~Hero();
	void Attack();
	void SetPosition(XMINT2 pos);
	void UpdateEntity(float deltaTime) override;
	class Component* GetHeroParam() { return mComponents[1]; }
	XMINT2 GetDir() { return mDirection; }
	bool GetIsMoving() { return mIsMoving; }
private:
	float mMoveSpeed = 5.0f;
	XMINT2 mDirection;
	XMINT2 mPrePos;
	bool mIsMoving;
};