#pragma once

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ObjectFactory.h
Purpose: Object Factory
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "Managers/ResourceManager.h"
#include <iostream>
#include <stdio.h>
#include "Managers/InputManager.h"
#include "Managers/FrameRateController.h"
#include "GameObject.h"
#include "Managers/GameObjectManager.h"
#include "Components/Transform.h"
#include "Components/Sprite.h"
#include "Components/Component.h"
#include "Components/Body.h"
#include "Components/Controller.h"
#include "Components/UpDown.h"
#include "Components/Button.h"
#include "Components/Target.h"
#include "Components/AI.h"
#include "Components/Text.h"
#include "Components/Expand.h"

class ObjectFactory
{
public:
	ObjectFactory()
		: mNoOfEnemyShip(0), mNoOfMeteors(0), mNoOfStars(0), levelPool(""), meteorsDestroyed(0), meteorsAlive(0), totalMeteors(0),
		isBulletAllowed(false), isHomingAllowed(false), isNuclearAllowed(false), isEnemnyShipAllowed(false),
		isEnemyShipAlive(false), win(false), mNoOfHoming(0), mNoOfNuclear(0)
	{}
	inline ~ObjectFactory() {}
	void update();

	void LoadLevelMeta(const char*);
	void LoadLevel(const char*);
	void LoadObjectPrefab(const char*);
	GameObject *LoadObject(const char*);
	void initiateLevel(const char*);
	void changeLevel(const char*);
	void nextLevel();

	void createBullet(float x, float y, unsigned int type, GameObject* pPlayer = nullptr);
	void createObjectAtShip(std::string file);
	void createMeteors();
	void createStars();
	void deleteObject(GameObject*);
	void destroyPause();

public:
	unsigned int mNoOfStars;
	std::string levelPool;

	unsigned int meteorsDestroyed;
	unsigned int meteorsAlive;
	bool isEnemyShipAlive;
	bool win;
	unsigned int mNoOfHoming;
	unsigned int mNoOfNuclear;

	// JSON LEVEL AND PREFABS
	std::unordered_map<std::string, Document*> mLevelData;
	std::unordered_map<std::string, Document*> mPrefabs;

	// OBJECT CREATION AND DELETION
	std::vector<GameObject*> newObjects;
	std::vector<GameObject*> deleteObjects;
	
	std::string currentLevel;

private:
	void initializeMembers();
	void spawnEnemyShip();

	unsigned int mNoOfEnemyShip;
	unsigned int mNoOfMeteors;
	
	int totalMeteors;
	float meteorVelocity;
	bool isBulletAllowed;
	bool isHomingAllowed;
	bool isNuclearAllowed;
	bool isEnemnyShipAllowed;

	std::vector<std::string> GameLevels;
};