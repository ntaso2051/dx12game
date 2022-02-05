#pragma once
#include "Item.h"

class Weapon : public Item {
public:
	Weapon(class Game* game, XMFLOAT3 pos, Const::TexId::Type texId);
	~Weapon();
	void Adapt() override;
	void Deadapt() override;
	void Remove() override;
private:
};