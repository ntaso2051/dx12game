#pragma once
#include "Item.h"

class Food : public Item {
public:
	Food(class Game* game, XMFLOAT3 pos, Const::TexId::Type texId);
	~Food();
private:
	Item::Type mType;
};