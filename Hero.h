#pragma once
#include "Entity.h"
#include <DirectXMath.h>
#include "Const.h"

class Hero :public Entity {
public:
	Hero(class Game* game, XMFLOAT3 pos);
	Hero(class Game* game, XMFLOAT3 pos, class ParameterComponent* param);
	void Attack();
	void SetPosition(XMINT2 pos);
	void UpdateEntity(float deltaTime) override;
	class Component* GetHeroParam() { return mComponents[1]; }
	XMINT2 GetDir() { return mDirection; }
private:
	// ‚Æ‚è‚ ‚¦‚¸
	float move = 0.0f;
	XMINT2 mDirection;
};