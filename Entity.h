#pragma once
#include <vector>
#include <DirectXMath.h>
#include <typeinfo>
#include <string>

using namespace DirectX;

class Entity {
public:
	Entity(class Game* game);
	virtual ~Entity();

	void Update(float deltaTime);
	virtual void UpdateEntity(float deltaTime);

	void Input();
	virtual void EntityInput();

	class Component* GetComponent(std::string compName);
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);
	void UpdateComponents(float deltaTime);

	const int GetUpdateOrder() { return mUpdateOrder; }

	bool Comp(const Entity* l, const Entity* r) { return (l->mUpdateOrder < r->mUpdateOrder); }

	XMINT2 GetPosition() { return mPosition; }
	class Game* GetGame();
protected:
	class Game* mGame;
	std::vector<class Component*>mComponents;
	// ç¿ïWä÷òA
	XMINT2 mPosition;
	int mUpdateOrder;
};