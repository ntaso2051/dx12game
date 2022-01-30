#pragma once
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;

class Entity {
public:
	Entity(class Game* game);
	virtual ~Entity();

	void Update(float deltaTime);
	virtual void UpdateEntity(float deltaTime);

	void Input();
	virtual void EntityInput();

	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);
	void UpdateComponents(float deltaTime);

	XMINT2 GetPosition() { return mPosition; }
	class Game* GetGame();
protected:
	class Game* mGame;
	std::vector<class Component*>mComponents;
	// ç¿ïWä÷òA
	XMINT2 mPosition;
};