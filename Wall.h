#pragma once
#include "Entity.h"

class Wall : public Entity {
public:
	Wall(class Game* game, XMFLOAT3 pos);
	~Wall();
private:
};