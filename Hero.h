#pragma once
#include "Entity.h"
#include <DirectXMath.h>

class Hero :public Entity {
public:
	Hero(class Game* game, XMFLOAT3 pos);
	void UpdateEntity(float deltaTime) override;
	class Component* GetHeroParam() { return mComponents[1]; }
private:
	// �Ƃ肠����
	float move = 0.0f;
};