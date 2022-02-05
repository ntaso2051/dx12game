#pragma once
#include "Entity.h"
#include <DirectXMath.h>
#include "Const.h"

class Hero :public Entity {
public:
	Hero(class Game* game, XMFLOAT3 pos);
	Hero(class Game* game, XMFLOAT3 pos, class ParameterComponent* param);
	~Hero();
	void Attack();
	void SetPosition(XMINT2 pos);
	void UpdateEntity(float deltaTime) override;
	XMINT2 GetDir() { return mDirection; }

	void PushItem(class Item* item) { mItems.push_back(item); }
	void RemoveItem(class Item* item);
	void UseItem(class Item* item);

	std::vector<class Item*> GetMyItems() { return mItems; }

	enum Equipment {
		Weapon,
		Shield,
		Max,
	};
private:
	float mMoveSpeed = 5.0f;
	XMINT2 mDirection;
	XMINT2 mPrePos;
	Const::State mState;
	std::vector<class Item*>mItems;
	std::vector<bool> mIsAttached;
};