#pragma once
#include "Entity.h"
#include <DirectXMath.h>

class EnemyBlob : public Entity {
public:
	EnemyBlob(class Game* game, XMFLOAT3 pos);
	~EnemyBlob();
private:
};
