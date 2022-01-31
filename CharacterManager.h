#pragma once
#include <vector>

class CharacterManager {
public:
	CharacterManager(class Hero* hero);
	~CharacterManager();
	enum Phase {
		HeroPhase,
		EnemiesPhase,
	};
	void ChangePhase();
	Phase GetPhase() { return mPhase; }
	void AddEnemyBlob(class EnemyBlob* enemyBlob);
	void RemoveEnemyBlob(class EnemyBlob* enemyBlob);
	void DamageCalc(class ParameterComponent* attack, class ParameterComponent* damaged);

private:
	class Hero* mHero;
	std::vector<class EnemyBlob*> mEnemyBlobs;
	Phase mPhase;
};