#pragma once
#include "Entity.h"
#include "Const.h"


class Item : public Entity {
public:
	Item(class Game* game, XMFLOAT3 pos, Const::TexId::Type texId);
	virtual ~Item();
	enum Type {
		Weapon,
		Once,
	};
	virtual void UpdateEntity(float deltaTime) override;
protected:
	class SpriteComponent* mSpriteComponent;
};