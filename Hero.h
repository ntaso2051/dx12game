#pragma once
#include "Entity.h"
#include <DirectXMath.h>

class Hero :public Entity {
public:
	Hero(class Game* game, XMFLOAT3 pos);
	void UpdateEntity(float deltaTime) override;
	class Component* GetHeroParam() { return mComponents[1]; }
private:
	// ‚Æ‚è‚ ‚¦‚¸
	float move = 0.0f;
};