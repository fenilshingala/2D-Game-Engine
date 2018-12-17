#pragma once

#include "Component.h"

class UpDown : public Component
{
public:
	UpDown();
	inline ~UpDown() {}
	void update();
	void Serialize(GenericObject<false, Value::ValueType>);
public:
	bool mGoingUp;
	int mTimer, mTimerLimit;
};