#pragma once

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObject.h
Purpose: Game Object
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include <vector>

class Component;
class Event;

enum GAME_OBJECT_TYPE {
	PLAYER,
	METEOR,
	BULLET,
	ENEMYBULLET,
	HOMINGMISSILE,
	NUCLEARMISSILE,
	RECT_BUTTON,
	STAR,
	ENEMYSHIP,
	TEXTOBJ,
	SHIELD
};

class GameObject
{
public:
	GameObject();
	~GameObject();

	void update();
	Component* AddComponent(unsigned int Type);
	Component* GetComponent(unsigned int Type);
	void HandleEvent(Event *pEvent);
public:
	unsigned int mType;
	bool mFlag;
	std::vector<Component *> mComponents;
};