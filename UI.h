#pragma once
#include "Entity.h"

class UI :public Entity {
public:
	UI(class Game* game);
	~UI();
	void UpdateEntity(float deltaTime)override;
	void Activate(int floor);
	void Deactivate();
private:
	float mDisplayTime = 10.0f;
	const float mDisplayTimeMax = 3.0f;
	class UIRenderComponent* mUrc;
	class UIRenderComponent* mUIB;
	class UIRenderComponent* mUIF;
	std::vector<class UIRenderComponent*> mNum0;
	std::vector<class UIRenderComponent*> mNum1;
};