#pragma once
#include "Item.h"
#include "Const.h"

class Clock :public Item {
public:
	void Init();
	Clock(class Game* game, XMFLOAT3 pos, Const::TexId::Type);
	Clock(class Game* game);
	~Clock();
	void Adapt() override;
	void Deadapt() override;
	void Remove() override;
};