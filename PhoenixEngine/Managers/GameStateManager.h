#pragma once

/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameStateManager.h
Purpose: Game State Manager
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include <stdio.h>
#include <string>

class GameStateManager
{
public:
	GameStateManager();
	~GameStateManager();

	void update();

	void setInitialLevel(const char* init) { 
		initialLevel = std::string(init);
	}
	inline void setNextLevel(const char* next) { nextLevel = next; }
	inline std::string getCurrentLevel() { return currentLevel; }

	std::string currentLevel;
private:
	std::string prevLevel;
	std::string nextLevel;
	std::string initialLevel;
};