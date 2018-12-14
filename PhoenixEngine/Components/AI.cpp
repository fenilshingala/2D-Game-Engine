/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: AI.cpp
Purpose: AI Component
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "AI.h"

#include "Body.h"
#include "../GameObject.h"
#include "../ObjectFactory.h"
#include "Transform.h"
#include "../Managers/InputManager.h"
#include "../Managers/FrameRateController.h"
#include "../Managers/GameObjectManager.h"

extern ObjectFactory *pObjectFactory;
extern InputManager* pInputManager;
extern FrameRateController* FRC;
extern GameObjectManager* pGameObjectManager;

AI::AI() : Component(OBJAI), noOfBullets(10), mTimer(4.0f), mpPlayer(pGameObjectManager->mPlayer) {}

AI::~AI() {}

void AI::update()
{
	if (mpOwner->mType == ENEMYSHIP) {
		mTimer -= FRC->getFrameTime() / 1000.0f;
		if (mTimer < 0.0f) {
			Transform* pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
			if (0 != noOfBullets) {
				pObjectFactory->createBullet(pTr->mPosX, pTr->mPosY-pTr->scaleY, BULLET, mpPlayer);
				--noOfBullets;
			}
			mTimer = 4.0f;
		}
	}
}