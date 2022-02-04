#pragma once
#include "Item.h"

class Weapon : public Item {
public:
	Weapon(class Game* game, XMFLOAT3 pos, Const::TexId::Type texId);
	~Weapon();
private:
	Item::Type mType;
};