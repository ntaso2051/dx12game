#pragma once
#include <vector>
#include <DirectXMath.h>
#include "Const.h"

using namespace DirectX;

class CharacterManager {
public:
	CharacterManager(class Hero* hero);
	~CharacterManager();
	void Update();
	enum Phase {
		HeroPhase,
		EnemiesPhase,
	};
	void ChangePhase();
	class Entity* FindEnemyByPos(XMINT2 pos);
	void AttackRequest(XMFLOAT3 pos, XMINT2 dir);
	void AttackRequestByEnemy(class Component* param);
	Phase GetPhase() { return mPhase; }
	void AddEnemy(class Enemy* enemy);
	void EraseEnemy(Enemy* en);
	void RemoveEnemy(class Entity* enemy);
	void DamageCalc(class ParameterComponent* attack, class ParameterComponent* damaged);
	std::vector<class Enemy*> GetEnemies() { return mEnemies; }
	std::vector<class Enemy*> GetDeadEnemies() { return mDeadEnemies; }

	int GetEnemiesTotal() { return mEnemiesTotal; }
	int GetEnemiesCnt() { return mEnemiesCount; }

	void IncEnemyCnt() { mEnemiesCount++; }
	void DecEnemiesTotal() { mEnemiesTotal--; }
	void IncEnemiesTotal() { mEnemiesTotal++; }

private:
	class Hero* mHero;
	std::vector<class Enemy*> mEnemies;
	Phase mPhase;
	int mEnemiesCount;
	int mEnemiesTotal;
	std::vector<class Enemy*>mDeadEnemies;
};