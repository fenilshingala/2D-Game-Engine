#pragma once

class GameObject;
#include "Component.h"


class Target : public Component
{
public:
	Target();
	~Target();

	void update();
	void Serialize(GenericObject<false, Value::ValueType>);
public:
	GameObject* mpTarget;

private:
	void findTarget();
};