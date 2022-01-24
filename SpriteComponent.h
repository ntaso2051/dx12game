#pragma once
#include "Component.h"

class SpriteComponent : public Component {
public:
	SpriteComponent(class Entity* owner, int textureId, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw();
	void Update(float deltaTime) override;
	int GetDrawOrder() { return mDrawOrder; };
	class SpriteRenderer* GetSpriteRenderer() { return mSpriteRenderer; }
protected:
	class SpriteRenderer* mSpriteRenderer;
	int mDrawOrder;
};