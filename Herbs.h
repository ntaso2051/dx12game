#pragma once
#include "Item.h"

class Herbs : public Item {
public:
	Herbs(class Game* game, XMFLOAT3 pos, Const::TexId::Type texId);
	Herbs(class Game* game);
	~Herbs();
	void Adapt() override;
	void Deadapt() override;
	void Remove() override;
	void Init();
private:
	const int mHealHp = 10;
};