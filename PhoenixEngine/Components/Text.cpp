/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Text.cpp
Purpose: Text Component
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "Text.h"

#include "../Managers/FrameRateController.h"
#include "../TextRenderer.h"
#include "../ObjectFactory.h"

extern FrameRateController* FRC;
extern TextRenderer* pTextRenderer;
extern ObjectFactory* pObjectFactory;

Text::Text() : Component(TEXT), t(new TextAnim("", 0.0f, 0)), mPosX(0.0f), mPosY(0.0f), mScale(0.0f),
				colorX(1.0f), colorY(1.0f), colorZ(1.0f)
{}

Text::~Text() { delete t; }

void Text::drawText()
{
	if (!destroyCalled) {
		if (t->update())
			pTextRenderer->RenderText(t->currentString, mPosX, mPosY, mScale, glm::vec3(colorX, colorY, colorZ));
		else if (!t->update()) {
			destroyCalled = true;
			pObjectFactory->deleteObject(mpOwner);
		}
	}
}

bool TextAnim::update()
{
	if (startTime <= 0.0f) {
		if (!isAnim)
			notDone = true;
		else if (isAnim) {
			if (0 == count)
				notDone = false;
			if (0 != count && notDone) {
				currentTime -= FRC->getFrameTime() / 1000.0f;
				if (currentTime < 0.0f) {
					if (i != length)
						currentString += str[i++];
					else if (i == length) {
						i = 0;
						currentString = "";
						if (-1 != count) {
							--count;
						}
					}
					currentTime = mTimer;
				}
			}
		}
	}
	else if(startTime > 0)
	{
		startTime -= FRC->getFrameTime() / 1000.0f;
	}

	if (lifetime != -1.0f) {
		lifetime -= FRC->getFrameTime() / 1000.0f;
		if (lifetime <= 0.0f) notDone = false;
	}

	return notDone;
}

void Text::Serialize(GenericObject<false, Value::ValueType> obj)
{
	t->str = obj["string"].GetString();
	mPosX = obj["posx"].GetFloat();
	mPosY = obj["posy"].GetFloat();
	mScale = obj["scale"].GetFloat();
	colorX = obj["colorx"].GetFloat();
	colorY = obj["colory"].GetFloat();
	colorZ = obj["colorz"].GetFloat();

	t->mTimer = obj["timer"].GetFloat();
	t->count = obj["count"].GetInt();
	t->isAnim = obj["isAnim"].GetBool();

	if (obj.HasMember("startTime"))
		t->startTime = obj["startTime"].GetFloat();
	if (obj.HasMember("lifetime"))
		t->lifetime = obj["lifetime"].GetFloat();


	if (!t->isAnim)
		t->currentString = t->str;
	
	t->currentTime = t->mTimer;
	t->length = t->str.length();
}