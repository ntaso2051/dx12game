#pragma once
#include "Component.h"
#include <vector>

class AnimSpriteComponent : public Component {
public:
	AnimSpriteComponent(class Entity* owner, std::vector<std::vector<int>> texIdss, int drawOrder = 100);
	~AnimSpriteComponent();
	void Update(float deltaTime) override;
	void SetId(int id) { mId = id; }
private:
	std::vector<std::vector<class SpriteRenderer*>>mSpriteRenderers;
	class SpriteComponent* mSpriteComponent;
	float frame;
	int mId;
};