#pragma once
#include "Component.h"

class UIRenderComponent :public Component {
public:
	UIRenderComponent(class Entity* owner, int texId, float w, float h, float x, float y);
	~UIRenderComponent() override;
	void SetActive(bool ok) { mActiveSelf = ok; }
	bool ActiveSelf() { return mActiveSelf; }
	void Draw();
	void Update(float deltaTime) override;
private:
	class SpriteRenderer* mSpriteRenderer;
	bool mActiveSelf;
	float mWidth;
	float mHeight;
	float mPosX;
	float mPosY;
};