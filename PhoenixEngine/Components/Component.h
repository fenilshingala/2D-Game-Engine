#pragma once

class GameObject;
#include <stdio.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <iostream>
#include <rapidjson/filereadstream.h>
using namespace rapidjson;

class Event;

enum COMPONENT_TYPE
{
	TRANSFORM,
	SPRITE,
	CONTROLLER,
	UPDOWN,
	BODY,
	UP,
	BUTTON,
	TARGET,
	OBJAI,
	TEXT,
	EXPAND
};

class Component
{
public:
	Component(unsigned int Type);
	virtual ~Component() {}

	virtual void update() = 0;
	inline unsigned int getType() { return mType; }

	virtual void Serialize(GenericObject<false, Value::ValueType>) {}
	virtual void HandleEvent(Event *pEvent) {}
public:
	GameObject *mpOwner;
private:
	unsigned int mType;
};
