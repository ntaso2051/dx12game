#pragma once
#include "Entity.h"
#include <DirectXMath.h>

class EnemyBlob : public Entity {
public:
	EnemyBlob(class Game* game, XMFLOAT3 pos);
	~EnemyBlob();
	void UpdateEntity(float deltaTime) override;
	class Component* GetSpriteComponent() { return (mComponents[0]); }
	class Component* GetParam() { return mComponents[1]; }
private:
};
