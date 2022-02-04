#pragma once
#include "Component.h"

class SpriteComponent : public Component {
public:
	SpriteComponent(class Entity* owner, int textureId, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw();
	void SetActive(bool active);
	void Update(float deltaTime) override;
	int GetDrawOrder() { return mDrawOrder; };
	class SpriteRenderer* GetSpriteRenderer() { return mSpriteRenderer; }
	void SetSpriteRenderer(class SpriteRenderer* sr) { mSpriteRenderer = sr; }
	bool ActiveSelf() { return mActiveSelf; }
protected:
	class SpriteRenderer* mSpriteRenderer;
	int mDrawOrder;
	bool mActiveSelf;
};