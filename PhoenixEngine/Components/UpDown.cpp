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