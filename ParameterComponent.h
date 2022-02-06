#pragma once
#include "Component.h"
class ParameterComponent : public Component {
public:
	ParameterComponent(class Entity* owner, int hp, int exp, int level, int attack);
	~ParameterComponent();

	void Update(float deltaTime) override;

	void Damaged(ParameterComponent* pc);
	void LevelUp();
	void UpdateStatus();
	void SetMaxHp(int hp) { mMaxHp = hp; }
	void SetHp(int hp) { mHp = hp % (mMaxHp + 1); }
	void SetExp(int exp) { mExp = exp; }
	void SetLevel(int level) { mLevel = level; }
	void SetAttack(int attack) { mAttack = attack; }
	void SetHunger(int hunger) { mHunger = hunger; }
	int GetMaxHp() { return mMaxHp; }
	int GetHp() { return mHp; }
	int GetExp() { return mExp; }
	int GetLevel() { return mLevel; }
	int GetAttack() { return mAttack; }
	int GetHunger() { return mHunger; }

protected:
	int mMaxHp;
	int mHp;
	int mExp;
	int mLevel;
	int mAttack;
	int mHunger;
};