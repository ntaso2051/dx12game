#pragma once
#include "Entity.h"

class UI :public Entity {
public:
	UI(class Game* game);
	~UI();
	void UpdateEntity(float deltaTime)override;
	void Activate(int floor);
	void Deactivate();
	void ActivateTitle();
	void DeactivateTitle();
private:
	float mDisplayTime = 3.0f;
	const float mDisplayTimeMax = 3.0f;
	class UIRenderComponent* mUrc;
	class UIRenderComponent* mUIB;
	class UIRenderComponent* mUIF;
	std::vector<class UIRenderComponent*> mNum0;
	std::vector<class UIRenderComponent*> mNum1;
	class UIRenderComponent* mTitle;
	class UIRenderComponent* mTitleSub;
};