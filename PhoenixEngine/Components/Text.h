#pragma once

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

