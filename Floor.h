#pragma once
#include "Entity.h"

class Floor :public Entity {
public:
	Floor(class Game* game, XMFLOAT3 pos);
	~Floor();
};