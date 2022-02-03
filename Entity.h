#pragma once
#include <vector>
#include <DirectXMath.h>
#include <typeinfo>
#include <string>

using namespace DirectX;

class Entity {
public:
	Entity(class Game* game);
	Entity(Game* game, XMFLOAT3 pos);
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

	XMFLOAT3 GetPosition() { return mPosition; }
	class Game* GetGame();
protected:
	class Game* mGame;
	std::vector<class Component*>mComponents;
	// ç¿ïWä÷òA
	XMFLOAT3 mPosition;
	int mUpdateOrder;
};