/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: UpDown.cpp
Purpose: UpDown Component
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "UpDown.h"
#include "..\Managers\FrameRateController.h"
#include "Transform.h"
#include "..\GameObject.h"

extern FrameRateController* FRC;

UpDown::UpDown() : Component(UPDOWN), mGoingUp(true),  mTimer(0),  mTimerLimit(1000) {}

void UpDown::update()
{
	mTimer += FRC->getFrameTime();
	if (mTimer > mTimerLimit) {
		mGoingUp = !mGoingUp;
		mTimer = 0;
	}

	Transform *pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));

	if (nullptr != pTr) {
		if (true == mGoingUp)
			pTr->mPosY -= 1.0166f;
		else pTr->mPosY += 1.0166f;
	}
}

void UpDown::Serialize(GenericObject<false, Value::ValueType> obj)
{
	int goingUp = obj["isGoingUp"].GetBool();
	int timerLimit = obj["Timer"].GetInt();

	mGoingUp = (bool)goingUp;
	mTimerLimit = timerLimit;
}