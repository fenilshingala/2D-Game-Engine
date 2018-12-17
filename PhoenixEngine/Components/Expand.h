#pragma once

#include "Component.h"

class Expand : public Component
{
public:
	Expand();
	~Expand();
	void update();
	void Serialize(GenericObject<false, Value::ValueType>);
	void HandleEvent(Event *pEvent);

	bool collideCalled;
};