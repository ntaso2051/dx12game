#pragma once
#include <vector>
#include <DirectXMath.h>
#include "Const.h"

using namespace DirectX;

class CharacterManager {
public:
	CharacterManager(class Hero* hero);
	~CharacterManager();
	enum Phase {
		HeroPhase,
		EnemiesPhase,
	};
	void ChangePhase();
	class Entity* FindEnemyByPos(XMINT2 pos);
	void AttackRequest(XMFLOAT3 pos, XMINT2 dir);
	Phase GetPhase() { return mPhase; }
	void AddEnemy(class Enemy* enemy);
	void RemoveEnemy(class Entity* enemy);
	void DamageCalc(class ParameterComponent* attack, class ParameterComponent* damaged);
	std::vector<class Enemy*> GetEnemies() { return mEnemies; }

private:
	class Hero* mHero;
	std::vector<class Enemy*> mEnemies;
	Phase mPhase;
};