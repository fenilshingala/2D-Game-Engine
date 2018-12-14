/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Target.cpp
Purpose: Target Component
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "Target.h"

#include "../GameObject.h"
#include "../Managers/GameObjectManager.h"
#include "../Components/Transform.h"
#include "../Components/Body.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062

extern GameObjectManager* pGameObjectManager;

Target::Target() : Component(TARGET), mpTarget(nullptr)
{}

Target::~Target()
{}

void Target::update()
{
	if (nullptr != mpTarget) {
		if (true == mpTarget->mFlag) {
			Body *pBody = static_cast<Body*>(mpOwner->GetComponent(BODY));
			Body *pTargetBody = static_cast<Body*>(mpTarget->GetComponent(BODY));
			Transform *pTargetTr = static_cast<Transform*>(mpTarget->GetComponent(TRANSFORM));

			Transform* pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));

			glm::vec2 v1(pBody->mVelX, pBody->mVelY);
			glm::vec2 v2( (pTargetBody->mPosX) - pBody->mPosX,
						  (pTargetBody->mPosY) - pBody->mPosY);

			glm::mat4 rotMat;
			glm::vec2 rotatedV1 = glm::vec2(glm::rotate(rotMat, (float)PI / 2, glm::vec3(0, 0, 1)) * glm::vec4(v1, 0, 0));

			if (glm::dot(rotatedV1, v2) < 0.0f)
				pTr->mAngle -= (float)(PI / 2.0f) * 0.016666f;
			else
				pTr->mAngle += (float)(PI / 2.0f) * 0.016666f;

			pBody->mVelX = cosf(pTr->mAngle) * 300000.0f;
			pBody->mVelY = sinf(pTr->mAngle) * 300000.0f;
		}
		else
			findTarget();
	}
	else
		findTarget();
}

void Target::Serialize(GenericObject<false, Value::ValueType> json)
{
	findTarget();
}

void Target::findTarget()
{
	float distance = FLT_MAX;

	GameObject* ship = nullptr;

	unsigned int size = pGameObjectManager->mGameObjects.size();
	for (unsigned int i = 0; i < size; ++i) {
		GameObject* go = pGameObjectManager->mGameObjects[i];
		if (true == go->mFlag) {
			if (go->mType == PLAYER) { ship = go; }
			if (go->mType == METEOR) {
				Transform* pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
				Transform* pTrGo = static_cast<Transform*>(go->GetComponent(TRANSFORM));

				float tempDistance = (float)sqrt(((pTr->mPosX - pTrGo->mPosX) * (pTr->mPosX - pTrGo->mPosX))
					+ ((pTr->mPosY - pTrGo->mPosY) * (pTr->mPosY - pTrGo->mPosY)));

				distance = (tempDistance < distance) ? tempDistance : distance;
				if (distance == tempDistance) {
					mpTarget = go;
				}
			}
		}
	}

}