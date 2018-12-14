#pragma once

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Componenet.h
Purpose: Component Class
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

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
