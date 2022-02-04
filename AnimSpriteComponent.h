#pragma once
#include "Component.h"
#include <vector>

class AnimSpriteComponent : public Component {
public:
	AnimSpriteComponent(class Entity* owner, std::vector<int> texIds, int drawOrder = 100);
	~AnimSpriteComponent();
	void Update(float deltaTime) override;
private:
	std::vector<class SpriteRenderer*>mSpriteRenderers;
	class SpriteComponent* mSpriteComponent;
	float frame;
};