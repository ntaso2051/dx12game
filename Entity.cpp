#include "Entity.h"
#include "Game.h"
#include "Component.h"

Entity::Entity(Game* game) :mGame(game) {
	mGame->AddEntity(this);
}

Entity::Entity(Game* game, XMFLOAT3 pos) : mGame(game), mPosition(pos) {
	mGame->AddEntity(this);
}

Entity::~Entity() {
	mGame->RemoveEntity(this);
	for (auto comp : mComponents) {
		delete comp;
	}
	mComponents.clear();
}

Game* Entity::GetGame() { return mGame; }

void Entity::Update(float deltaTime) {
	UpdateComponents(deltaTime);
	UpdateEntity(deltaTime);
}

void Entity::UpdateEntity(float deltaTime) {

}

void Entity::Input() {

}

void Entity::EntityInput() {

}

Component* Entity::GetComponent(std::string compName) {
	std::string cn = "class " + compName;
	for (auto comp : mComponents) {
		if (typeid(*comp).name() == cn) {
			return comp;
		}
	}
	return nullptr;
}

void Entity::AddComponent(Component* component) {
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (; iter != mComponents.end(); iter++) {
		if (myOrder < (*iter)->GetUpdateOrder()) {
			break;
		}
	}
	mComponents.insert(iter, component);
}

void Entity::RemoveComponent(Component* component) {
	for (auto it = mComponents.begin(); it != mComponents.end();) {
		if (*it == component) {
			it = mComponents.erase(it);
		}
		else {
			++it;
		}
	}
}

void Entity::UpdateComponents(float deltaTime) {
	for (auto comp : mComponents) {
		comp->Update(deltaTime);
	}
}