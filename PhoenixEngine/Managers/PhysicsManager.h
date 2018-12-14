#pragma once

/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: PhysicsManager.h
Purpose: Physics Manager
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "../Managers/EventManager.h"
class Body;

class CollideEvent : public Event {
public:
	CollideEvent() : Event(COLLIDE) {}		~CollideEvent() {}
};

class DestroyEvent : public Event {
public:
	DestroyEvent() : Event(DESTROY) {}		~DestroyEvent() {}
};

class MeteorHitEvent : public Event {
public:
	MeteorHitEvent() : Event(METEORHIT) {}		~MeteorHitEvent() {}
};

class GameOverEvent : public Event {
public:
	GameOverEvent() : Event(GAMEOVER) {}		~GameOverEvent() {}
};

class ShieldOver : public Event {
public:
	ShieldOver() : Event(SHIELDOVER) {}		~ShieldOver() {}
};

class PhysicsManager
{
public:
	PhysicsManager();		~PhysicsManager();
	PhysicsManager(const PhysicsManager&) = delete;
	PhysicsManager& operator=(const PhysicsManager&) = delete;
	void update(float frameTime);
private:
	//std::unordered_map<std::pair<Body*, Body*>, EventType> eventBodyList;
};