#pragma once
#include <vector>

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

	class Game* GetGame();
private:
	class Game* mGame;
	std::vector<class Component*>mComponents;
};