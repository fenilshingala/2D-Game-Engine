/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameStateManager.cpp
Purpose: Game State Manager
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "GameStateManager.h"

#include "../ObjectFactory.h"

extern ObjectFactory* pObjectFactory;

GameStateManager::GameStateManager() : currentLevel(""), initialLevel(""), prevLevel(""), nextLevel("")
{}

GameStateManager::~GameStateManager()
{}

void GameStateManager::update()
{
	if (initialLevel != "") {
		prevLevel = nextLevel = currentLevel = initialLevel;
		pObjectFactory->initiateLevel(currentLevel.c_str());
		initialLevel = "";
	}

	if (currentLevel != nextLevel) {
		prevLevel = currentLevel;
		currentLevel = nextLevel;
		pObjectFactory->changeLevel(currentLevel.c_str());
	}
}