#pragma once

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