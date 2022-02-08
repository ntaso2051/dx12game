#pragma once
#include "Item.h"

class Bomb : public Item {
public:
	void Init();
	Bomb(class Game* game, XMFLOAT3 pos, Const::TexId::Type texId);
	Bomb(class Game* game);
	~Bomb();
	void Adapt() override;
	void Deadapt() override;
	void Remove() override;
private:
};