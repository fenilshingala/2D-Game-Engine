#pragma once

#include "Component.h"

class Transform : public Component
{
public:
	Transform();
	inline ~Transform() {}
	inline void update() {}

	void Serialize(GenericObject<false, Value::ValueType>);
public:
	float mPosX;
	float mPosY;
	float scaleX;
	float scaleY;
	float mAngle;
};

