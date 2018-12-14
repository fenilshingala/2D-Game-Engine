/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Button.cpp
Purpose: Button Component
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "Button.h"

#include "../Managers/InputManager.h"
#include "../GameObject.h"
#include "Transform.h"
#include "../Managers/GameStateManager.h"
#include "../ObjectFactory.h"
#include <irrKlang.h>
using namespace irrklang;

#include <iostream>

extern InputManager* pInputManager;
extern EventManager* pEventManager;
extern GameStateManager* pGameStateManager;
extern ObjectFactory *pObjectFactory;
extern ISoundEngine *SoundEngine;

extern bool appIsRunning;
extern int width;
extern int height;
extern bool isGamePaused;
extern bool isGameStopped;

Button::Button() : Component(BUTTON) {}

Button::~Button() {}

void Button::HandleEvent(Event* pEvent)
{
	if (pEvent->mType == PRESSED) {
		if (mGameMode == START)
			pObjectFactory->nextLevel();
		else if (mGameMode == QUIT || mGameMode == PAUSE_QUIT)
			appIsRunning = false;
		else if (mGameMode == HOWTOPLAY)
			pGameStateManager->setNextLevel("HowToPlay.json");

		else if (mGameMode == PAUSE_CONTINUE)
		{
			pObjectFactory->destroyPause();
			isGamePaused = false;
		}
		else if (mGameMode == MAINMENU || mGameMode == PAUSE_MAINMENU)
			pObjectFactory->levelPool = "Home.json";
		else if (mGameMode == RESTART)
		{
			pObjectFactory->changeLevel( pObjectFactory->currentLevel.c_str() );
			isGameStopped = false;
		}

		else if (mGameMode == SKIP)
		{
			pObjectFactory->nextLevel();
		}

		else if (mGameMode == CREDITS)
		{
			pGameStateManager->setNextLevel("Credits.json");
		}

	}
}

void Button::update()
{
	if(pInputManager->isButtonTriggerred(SDL_BUTTON_LEFT))
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		
		y = height-y;

		Transform* pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
		if (
			x >= (pTr->mPosX - pTr->scaleX/2.0f)
		 && x <= (pTr->mPosX + pTr->scaleX/2.0f)
		 && y >= (pTr->mPosY - pTr->scaleY/2.0f)
		 && y <= (pTr->mPosY + pTr->scaleY/2.0f)
		)
		{
			PressedEvent* pe = new PressedEvent;
			pe->mTimer = 0.0f;
			pEventManager->AddTimeEvent(pe);
			pEventManager->Subscribe(PRESSED, mpOwner);
			SoundEngine->play2D("../Resources/Audio/buttonselect/5.wav", GL_FALSE);
		}
	}
}

void Button::Serialize(GenericObject<false, Value::ValueType> json)
{
	if (json.HasMember("name")) {
		std::string gameMode = json["name"].GetString();

		if (gameMode == "StartGame") { mGameMode = START; }
		else if (gameMode == "Quit") { mGameMode = QUIT; }
		else if (gameMode == "HowToPlay") { mGameMode = HOWTOPLAY; }
		else if (gameMode == "pContinue") { mGameMode = PAUSE_CONTINUE; }
		else if (gameMode == "pQuit") { mGameMode = PAUSE_QUIT; }
		else if (gameMode == "MainMenu") { mGameMode = MAINMENU; }
		else if (gameMode == "pMainMenu") { mGameMode = PAUSE_MAINMENU; }
		else if (gameMode == "TryAgain") { mGameMode = RESTART; }
		else if (gameMode == "Skip") { mGameMode = SKIP; }
		else if (gameMode == "Credits") { mGameMode = CREDITS; }
	}
}