#pragma once
#include "Entity.h"
#include "Const.h"


class Item : public Entity {
public:
	Item(class Game* game, XMFLOAT3 pos, Const::TexId::Type texId);
	virtual ~Item();
	enum Type {
		Equipment,
		Once,
	};
	virtual void UpdateEntity(float deltaTime) override;
	virtual void Adapt();
	virtual void Deadapt();
	virtual void Remove();

	std::string GetName() { return mName; }
	std::string GetInfo() { return mInfo; }

	int GetType() { return mType; }
protected:
	class SpriteComponent* mSpriteComponent;
	std::string mName;
	std::string mInfo;
	int mType;
};