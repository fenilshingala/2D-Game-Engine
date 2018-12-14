/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Body.cpp
Purpose: Body Component
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "Body.h"

#include "Transform.h"
#include "../GameObject.h"
#include "../Managers/CollisionManager.h"
#include "../Managers/PhysicsManager.h"
#include "../ObjectFactory.h"
#include "../Managers/FrameRateController.h"
#include "../TextRenderer.h"
#include "../Managers/GameObjectManager.h"
#include <irrKlang.h>
using namespace irrklang;

extern EventManager* pEventManager;
extern ObjectFactory* pObjectFactory;
extern FrameRateController* FRC;
extern TextRenderer* pTextRenderer;
extern ISoundEngine *SoundEngine;
extern GameObjectManager *pGameObjectManager;

extern int width;
extern int height;
extern bool isGamePaused;
extern bool isGameStopped;

Body::Body() : Component(BODY), mPosX(0.0f), mPosY(0.0f), mPrevPosX(0.0f), mPrevPosY(0.0f), mVelX(0.0f), mVelY(0.0f), mAccX(0.0f), mAccY(0.0f),
				mTotalForceX(0.0f), mTotalForceY(0.0f), mMass(0.0f), mInvMass(0.0f),
				deleteCalled(false), collideCalled(false), meteorHitCalled(false), mTimer(0.0f), life(0)
{}
Body::~Body(){}

void Body::update(){
	Transform* pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
	if (nullptr != pTr) {
		mPosX = pTr->mPosX;
		mPosY = pTr->mPosY;
	}

	if (mpOwner->mType == PLAYER) {
		if (meteorHitCalled) {
			mTimer -= FRC->getFrameTime() / 1000.0f;
			if (mTimer < 0) {
				//std::cout << "Time's up\n";
				pEventManager->BroadcastEventToSubscribers(new Event(SHIELDOVER));
				meteorHitCalled = false;
			}
		}
	}
	if (mpOwner->mType == SHIELD)
	{
		Transform *pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
		Transform *pPlTr = static_cast<Transform*>(pGameObjectManager->mPlayer->GetComponent(TRANSFORM));
		mPosX = pPlTr->mPosX;
		mPosY = pPlTr->mPosY;
	}
}

void Body::Serialize(GenericObject<false, Value::ValueType> obj)
{
	if(obj.HasMember("Mass"))
		mMass = obj["Mass"].GetFloat();

	if (0.0f != mMass)
		mInvMass = 1.0f / mMass;
	else
		mInvMass = 0.0f;

	if (obj.HasMember("Circle")) {
		mpShape = new ShapeCircle();
		mpShape->mpOwnerBody = this;
		ShapeCircle *pCircle = static_cast<ShapeCircle*>(mpShape);
	}

	if (obj.HasMember("AABB")) {
		mpShape = new ShapeAABB();
		mpShape->mpOwnerBody = this;
		ShapeAABB *pAABB = static_cast<ShapeAABB*>(mpShape);
	}
}

void Body::Initialize()
{
	Transform *pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));

	if (nullptr != pTr) {
		mPrevPosX = mPosX = pTr->mPosX;
		mPrevPosY = mPosY = pTr->mPosY;
		
		if (mpShape->mType == Shape::RECTANGLE) {
			static_cast<ShapeAABB*>(mpShape)->mRight = pTr->scaleX;
			static_cast<ShapeAABB*>(mpShape)->mTop = pTr->scaleY;
		}

		else if (mpShape->mType == Shape::CIRCLE)	static_cast<ShapeCircle*>(mpShape)->mRadius = pTr->scaleX/2.0f;
	}

	if (mpOwner->mType == SHIELD)	pEventManager->Subscribe(SHIELDOVER, mpOwner);

	if (mpOwner->mType == ENEMYSHIP)	life = 50;
	else if (mpOwner->mType == PLAYER)	life = 5;
}

void Body::Integrate(float Gravity, float deltaTime) {

	// Save current position
	mPrevPosX = mPosX;
	mPrevPosY = mPosY;

	// Compute acceleration
	mTotalForceY -= Gravity;

	mAccX = mTotalForceX * mInvMass;
	mAccY = mTotalForceY * mInvMass;

	// velocity
	Transform *pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
	mVelX = mAccX * deltaTime + mVelX;
	mVelY = mAccY * deltaTime + mVelY;

	if (mpOwner->mType == PLAYER) {
		mVelX *= 0.98f;
		mVelY = 0.0f;
	}

	if (mpOwner->mType == ENEMYSHIP) {
		if(mPosY <= height - (int)(2*pTr->scaleY))
			mVelY = 0.0f;
	}

	// position
	mPosX = mVelX * deltaTime + mPosX;
	mPosY = mVelY * deltaTime + mPosY;

	if (mpOwner->mType == PLAYER) {
		Wrap(mPosX, 0.0f, (float)width);
	}
	else if (mpOwner->mType == HOMINGMISSILE) {
		Wrap(mPosX, 0.0f, (float)width);
		Wrap(mPosY, 0.0f, (float)height);
	}
	else if (mpOwner->mType == BULLET || mpOwner->mType == NUCLEARMISSILE) {
		if (Wrap(mPosX, -pTr->scaleX * 2, (float)width+pTr->scaleX*2) ||
			Wrap(mPosY, -pTr->scaleY * 2, (float)height+ pTr->scaleY * 2))
		{
			DestroyEvent* pDE = new DestroyEvent();
			pDE->mTimer = 0.0f;
			pEventManager->AddTimeEvent(pDE);
			pEventManager->Subscribe(DESTROY, mpOwner);
		}
	}
	else if (mpOwner->mType == METEOR) {
		if (mPosY < -100.0f) {
			DestroyEvent* pDE = new DestroyEvent();
			pDE->mTimer = 0.0f;
			pEventManager->AddTimeEvent(pDE);
			pEventManager->Subscribe(DESTROY, mpOwner);
		}
	}
	

	mTotalForceX = mTotalForceY = 0.0f;

	if (nullptr != pTr) {
		pTr->mPosX = mPosX;
		pTr->mPosY = mPosY;
	}
}

bool Body::Wrap(float& entity, float min, float max)
{
	bool outside = false;
	if (entity > max || entity < min) { outside = true; }
	if (entity > max) { entity = min; }
	if (entity < min) { entity = max; }
	return outside;
}

void Body::HandleEvent(Event* pEvent)
{
	if (mpOwner->mType != PLAYER) {
		if (DESTROY == pEvent->mType)
		{
			if (false == deleteCalled) {
				pObjectFactory->deleteObject(mpOwner);
				deleteCalled = true;
				if (NUCLEARMISSILE == mpOwner->mType)
					SoundEngine->play2D("../Resources/Audio/explosions/5.wav", GL_FALSE);
			}
		}
	}

	if (COLLIDE == pEvent->mType)
	{
		if (mpOwner->mType == ENEMYSHIP) {
			if (life > 0) --life; 
			if (0 == life) 
				pObjectFactory->win = true;
		}
		if (false == collideCalled && SHIELD != mpOwner->mType && 0 == life) {
			DestroyEvent* pDE = new DestroyEvent();

			if (mpOwner->mType == METEOR) {
				++pObjectFactory->meteorsDestroyed;
				std::cout << "meteorsDestroyed: " << pObjectFactory->meteorsDestroyed << "\n";
			}

			pDE->mTimer = 0.0f;
			pEventManager->AddTimeEvent(pDE);
			pEventManager->Subscribe(DESTROY, mpOwner);
			collideCalled = true;
		}

	}

	if (METEORHIT == pEvent->mType)
	{

		if (mpOwner->mType == PLAYER) {
			if (false == meteorHitCalled) {
				meteorHitCalled = true;
				mTimer = 2.0f;	
				--life;
				
				pObjectFactory->createObjectAtShip("Shield.json");
				if (life <= 0) {
					if (!isGamePaused) {
						pObjectFactory->LoadLevel("GameOver.json");
						isGameStopped = true;
					}
				}
			}
		}

	}

	if (SHIELDOVER == pEvent->mType)
	{
		if (!deleteCalled) { pObjectFactory->deleteObject(mpOwner); deleteCalled = true; }
	}

}