#pragma once
#include "Component.h"
class ParameterComponent : public Component {
public:
	ParameterComponent(class Entity* owner, int hp, int exp, int level, int attack);
	~ParameterComponent();
	void Damaged(ParameterComponent* pc);


	void SetHp(int hp) { mHp = hp; }
	void SetExp(int exp) { mExp = exp; }
	void SetLevel(int level) { mLevel = level; }
	void SetAttack(int attack) { mAttack = attack; }
	int GetHp() { return mHp; }
	int GetExp() { return mExp; }
	int GetLevel() { return mLevel; }
	int GetAttack() { return mAttack; }
protected:
	int mHp;
	int mExp;
	int mLevel;
	int mAttack;
};