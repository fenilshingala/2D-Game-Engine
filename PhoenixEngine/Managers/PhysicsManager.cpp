/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: PhysicsManager.cpp
Purpose: Physics Manager
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "PhysicsManager.h"

#include "GameObjectManager.h"
#include "..\Components\Body.h"
#include "..\GameObject.h"
#include "CollisionManager.h"

extern GameObjectManager* pGameObjectManager;
extern CollisionManager *pCollisionManager;

PhysicsManager::PhysicsManager() {}
PhysicsManager::~PhysicsManager() {}

void PhysicsManager::update(float frameTime)
{
	unsigned int count = pGameObjectManager->mGameObjects.size();
	for (unsigned int i = 0; i < count; ++i) {

		if (true == pGameObjectManager->mGameObjects[i]->mFlag) {
			GameObject* pGameObject = pGameObjectManager->mGameObjects[i];
			if (nullptr != pGameObject) {
				Body *pBody = static_cast<Body*>(pGameObject->GetComponent(BODY));
				if (nullptr != pBody) {
					pBody->Integrate(10000.0f, frameTime / 1000.0f);
				}
			}
		}

	}

	pCollisionManager->Reset();

	for (unsigned int i = 0; i < count; ++i){

		Body *pBody1 = nullptr;

		if (true == pGameObjectManager->mGameObjects[i]->mFlag) {
			GameObject* pObj1 = pGameObjectManager->mGameObjects[i];
			pBody1 = static_cast<Body*>(pObj1->GetComponent(BODY));
			if (nullptr == pBody1) {
				continue;
			}
		}
		else { continue; }

		for (unsigned int j = i; j < count; ++j) {

			Body *pBody2 = nullptr;
			if (true == pGameObjectManager->mGameObjects[j]->mFlag) {
				GameObject* pObj2 = pGameObjectManager->mGameObjects[j];
				pBody2 = static_cast<Body*>(pObj2->GetComponent(BODY));
				if (nullptr == pBody2) {
					continue;
				}
			}
			else { continue; }

			if (pBody1->mpOwner->mType == pBody2->mpOwner->mType) continue;

			if ( (pBody1->deleteCalled && pBody1->mpOwner->mType!=NUCLEARMISSILE) ||
				 (pBody2->deleteCalled && pBody2->mpOwner->mType!=NUCLEARMISSILE) ) continue;

			if (pCollisionManager->ChechCollisionAndGenerate(
				pBody1->mpShape, pBody1->mPosX, pBody1->mPosY,
				pBody2->mpShape, pBody2->mPosX, pBody2->mPosY
				)
			)
			{
				// Super advanced physics here!
				for (auto c : pCollisionManager->mContacts)
				{
					if (
						((pBody1->mpOwner->mType == BULLET && pBody2->mpOwner->mType == METEOR)
						|| (pBody2->mpOwner->mType == BULLET && pBody1->mpOwner->mType == METEOR))
						
						||
						
						((pBody1->mpOwner->mType == HOMINGMISSILE && pBody2->mpOwner->mType == METEOR)
						|| (pBody2->mpOwner->mType == HOMINGMISSILE && pBody1->mpOwner->mType == METEOR))
						
						||

						((pBody1->mpOwner->mType == NUCLEARMISSILE && pBody2->mpOwner->mType == METEOR)
						|| (pBody2->mpOwner->mType == NUCLEARMISSILE && pBody1->mpOwner->mType == METEOR))
						
						||
						
						((pBody1->mpOwner->mType == BULLET && pBody2->mpOwner->mType == ENEMYSHIP)
						|| (pBody2->mpOwner->mType == BULLET && pBody1->mpOwner->mType == ENEMYSHIP))

						||

						((pBody1->mpOwner->mType == METEOR && pBody2->mpOwner->mType == SHIELD)
						|| (pBody2->mpOwner->mType == METEOR && pBody1->mpOwner->mType == SHIELD))

						||

						((pBody1->mpOwner->mType == ENEMYBULLET && pBody2->mpOwner->mType == SHIELD)
						|| (pBody2->mpOwner->mType == ENEMYBULLET && pBody1->mpOwner->mType == SHIELD))
					)
					{
						CollideEvent ce;
						c->mBodies[0]->mpOwner->HandleEvent(&ce);
						c->mBodies[1]->mpOwner->HandleEvent(&ce);
					}

					if (((pBody1->mpOwner->mType == METEOR && pBody2->mpOwner->mType == PLAYER)
						|| (pBody2->mpOwner->mType == METEOR && pBody1->mpOwner->mType == PLAYER))
						
						||

						((pBody1->mpOwner->mType == ENEMYBULLET && pBody2->mpOwner->mType == PLAYER)
						|| (pBody2->mpOwner->mType == ENEMYBULLET && pBody1->mpOwner->mType == PLAYER))
					)
					{
						//auto bodyPair = std::make_pair(pBody1, pBody2);
						//eventBodyList.insert<std::pair, EventType>(bodyPair, METEORHIT);
						//eventBodyList.insert(std::pair<std::pair<Body*,Body*>,EventType>(bodyPair , METEORHIT));
						//eventBodyList[bodyPair] = METEORHIT;//.insert(bodyPair, METEORHIT);

						MeteorHitEvent mh;
						c->mBodies[0]->mpOwner->HandleEvent(&mh);
						c->mBodies[1]->mpOwner->HandleEvent(&mh);
					}

				}
			}

		}

	}

	// Super advanced physics

}