#include "Entity.h"
#include "Game.h"
#include "Component.h"

Entity::Entity(Game* game) :mGame(game) {
	mGame->AddEntity(this);
}

Entity::~Entity() {
	mGame->RemoveEntity(this);
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
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end()) {
		mComponents.erase(iter);
	}
}

void Entity::UpdateComponents(float deltaTime) {
	for (auto comp : mComponents) {
		comp->Update(deltaTime);
	}
}