#pragma once
#include "Component.h"

class SpriteComponent : public Component {
public:
	SpriteComponent(class Entity* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw();
	void Update(float deltaTime) override;
	int GetDrawOrder() { return mDrawOrder; };
protected:
	class SpriteRenderer* mSpriteRenderer;
	int mDrawOrder;
};