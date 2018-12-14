/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Expand.cpp
Purpose: Expand Component
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "Expand.h"

#include "../Managers/PhysicsManager.h"
#include "../ObjectFactory.h"
#include "../GameObject.h"
#include "../Managers/EventManager.h"

extern EventManager* pEventManager;

Expand::Expand() : Component(EXPAND), collideCalled(false)
{
}


Expand::~Expand()
{
}

void Expand::update()
{
	Transform *pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
	if (collideCalled) {
		pTr->scaleX += 2.0f;
		pTr->scaleY += 2.0f;
	}
}

void Expand::Serialize(GenericObject<false, Value::ValueType>)
{}

void Expand::HandleEvent(Event *pEvent)
{
	if (COLLIDE == pEvent->mType)
	{
		if (false == collideCalled)
			collideCalled = true;
	}
}