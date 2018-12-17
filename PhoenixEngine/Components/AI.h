#pragma once

#include "../Components/Component.h"

class AI : public Component
{
public:
	AI();
	~AI();

	void update();

private:
	GameObject* mpPlayer;
	unsigned int noOfBullets;
	float mTimer;
};