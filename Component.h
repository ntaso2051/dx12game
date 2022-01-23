#pragma once

class Component {
public:
	Component(class Entity* owner, int updateOrder = 100);
	virtual ~Component();
	virtual void Update(float deltaTime);
	int GetUpdateOrder() { return mUpdateOrder; }
protected:
	class Entity* mOwner;
	int mUpdateOrder;
};