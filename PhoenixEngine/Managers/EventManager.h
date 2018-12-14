#pragma once

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

#include <list>
#include <unordered_map>

class GameObject;

enum EventType {
	PLAYER_HIT,
	COLLIDE,
	DESTROY,
	PRESSED,
	METEORHIT,
	GAMEOVER,
	SHIELDOVER,

	NUM
};

class Event {
public:
	Event(EventType type);
	virtual ~Event() {}

	EventType mType;
	float mTimer;
};

class EventManager
{
public:
	EventManager();
	~EventManager();
	void BoradCastEvent(Event* pEvent);
	void AddTimeEvent(Event* pEvent);

	void update(float);
	void Subscribe(EventType type, GameObject *pGameObject);
	void BroadcastEventToSubscribers(Event* event);
	void Unsubscribe(EventType type, GameObject *pGameObject);

	std::list<Event*> mTimedEvents;
	std::unordered_map<EventType, std::list<GameObject*>> mSubscription;
};