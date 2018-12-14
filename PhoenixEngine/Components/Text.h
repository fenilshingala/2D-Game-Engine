#pragma once

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Text.h
Purpose: Text Component
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "Component.h"
#include <string>

class TextAnim {
public:
	TextAnim(std::string _str, float timer, unsigned int _count)
		: mTimer(timer), currentString(""), str(_str), i(0), count(_count), notDone(true), isAnim(false),
			lifetime(-1.0f), startTime(-1.0f) {}
	float mTimer;
	float currentTime;
	std::string currentString;
	std::string str;
	unsigned int i;
	unsigned int length;
	int count;
	bool notDone;
	bool isAnim;
	float startTime;
	float lifetime;

	bool update();
};

class Text : public Component
{
public:
	Text();
	~Text();

	void Serialize(GenericObject<false, Value::ValueType>);
	void update() {}
	void drawText();

	TextAnim* t;
	float mPosX;
	float mPosY;
	float mScale;
	float colorX;
	float colorY;
	float colorZ;
	bool destroyCalled;
};

