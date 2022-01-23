#include "Component.h"
#include "Entity.h"

Component::Component(Entity* owner, int updateOrder) :mOwner(owner), mUpdateOrder(updateOrder) {
	mOwner->AddComponent(this);
}

Component::~Component() { mOwner->RemoveComponent(this); }

void Component::Update(float deltaTime) {}