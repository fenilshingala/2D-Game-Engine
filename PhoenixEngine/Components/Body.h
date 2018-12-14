#pragma once

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Body.h
Purpose: Body Component
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "Component.h"

class Shape;

class Body : public Component
{
public:
	Body();
	~Body();

	void update();
	void Serialize(GenericObject<false, Value::ValueType>);

	void Initialize();
	void Integrate(float Gravity, float deltaTime);
	void HandleEvent(Event *pEvent);
public:
	float mPosX, mPosY;
	float mPrevPosX, mPrevPosY;
	float mVelX, mVelY;
	float mAccX, mAccY;
	float mTotalForceX, mTotalForceY;
	float mMass, mInvMass;
	bool meteorHitCalled;
	bool deleteCalled;
	unsigned int life;

	Shape* mpShape;

private:
	bool Wrap(float&, float, float);
	bool collideCalled;
	float mTimer;
};