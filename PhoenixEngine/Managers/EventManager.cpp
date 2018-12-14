/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: EventManager.h
Purpose: Event Manager
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "EventManager.h"
#include "GameObjectManager.h"
#include "../GameObject.h"

extern GameObjectManager* pGameObjectManager;

Event::Event(EventType type) {
	mType = type;
	mTimer = 0.0f;
}

EventManager::EventManager() {}
EventManager::~EventManager() {}

void EventManager::BoradCastEvent(Event* pEvent)
{
	for (auto goc : pGameObjectManager->mGameObjects) {
		goc->HandleEvent(pEvent);
	}
}

void EventManager::AddTimeEvent(Event* pEvent)
{
	mTimedEvents.push_back(pEvent);
}

void BroadcastEvent(Event* pEvent) {
	for (auto pGo : pGameObjectManager->mGameObjects) {
		pGo->HandleEvent(pEvent);
	}
}

void EventManager::update(float frameTime)
{
	// loop all timed events
	// decrease timer by frametime
	// < 0 ? -> Fire Event

	std::list<Event* >::iterator it, itEnd;
	it = mTimedEvents.begin();

	while (it != mTimedEvents.end())
	{
		Event *pEvent = *it;
		pEvent->mTimer -= frameTime;
		if (pEvent->mTimer < 0.0f) {
			BroadcastEventToSubscribers(pEvent);
			delete pEvent;
			it = mTimedEvents.erase(it);
		}
		else
			++it;
	}
}

void EventManager::Subscribe(EventType type, GameObject *pGameObject)
{
	//std::list<GameObject *> &listOfSubscribers = mSubscription[type];
	auto &listOfSubscribers = mSubscription[type];

	for (auto pGo : listOfSubscribers) {
		if (pGo == pGameObject)
			return;
	}
	listOfSubscribers.push_back(pGameObject);
}

void EventManager::Unsubscribe(EventType type, GameObject *pGameObject)
{
	//std::list<GameObject *> &listOfSubscribers = mSubscription[type];
	auto &listOfSubscribers = mSubscription[type];

	auto it = std::find(listOfSubscribers.begin(),
		listOfSubscribers.end(),
		pGameObject);

	if (it == listOfSubscribers.end())
		return;

	listOfSubscribers.erase(it);
}

void EventManager::BroadcastEventToSubscribers(Event* pEvent) {
	auto &listOfSubscribers = mSubscription[pEvent->mType];

	for (auto pGo : listOfSubscribers) {
		if(pGo->mFlag)
			pGo->HandleEvent(pEvent);
	}
}