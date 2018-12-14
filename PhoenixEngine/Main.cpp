/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Main.cpp
Purpose: Main Function File
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "Managers/ResourceManager.h"
#include "Managers/InputManager.h"
#include "Managers/FrameRateController.h"
#include "Managers/GameObjectManager.h"
#include "GameObject.h"
#include "Components/Component.h"
#include "Components/Transform.h"
#include "Components/Sprite.h"
#include "ObjectFactory.h"

#include "Managers/PhysicsManager.h"
#include "Managers/CollisionManager.h"
#include "Managers/GameStateManager.h"

#include "Mesh.h"
#include "Shader.h"

#include "Renderer.h"
#include "Managers/EventManager.h"
#include "Managers/ParticleManager.h"
#include "TextRenderer.h"

#include <irrKlang.h>
using namespace irrklang;

#include <stdio.h>
#include <string.h>

FILE _iob[] = { *stdin, *stdout, *stderr };

InputManager* pInputManager = nullptr;
FrameRateController* FRC = nullptr;
ResourceManager* pResourceManager = nullptr;
GameObjectManager* pGameObjectManager = nullptr;
ObjectFactory *pObjectFactory = nullptr;
PhysicsManager *pPhysicsManager = nullptr;
CollisionManager *pCollisionManager = nullptr;
Renderer *pRenderer = nullptr;
EventManager* pEventManager = nullptr;
GameStateManager* pGameStateManager = nullptr;
ParticleManager* pParticleManager = nullptr;
TextRenderer* pTextRenderer = nullptr;
ISoundEngine *SoundEngine = nullptr;

int width = 800;
int height = 600;
float frameTime = 0.0f;
bool isGamePaused = false;
bool isGameStopped = false;
bool debugMode = false;

bool appIsRunning = NULL;

extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}

#pragma comment(lib, "legacy_stdio_definitions.lib")

int main(int argc, char* args[])
{
	SDL_Window *pWindow;
	int error = 0;
	appIsRunning = true;

	width = 800;
	height = 600;

	pGameObjectManager = new GameObjectManager();
	pInputManager = new InputManager();
	FRC = new FrameRateController(60);
	pObjectFactory = new ObjectFactory();
	pPhysicsManager = new PhysicsManager();
	pCollisionManager = new CollisionManager();
	pRenderer = new Renderer();
	pEventManager = new EventManager();
	pGameStateManager = new GameStateManager();
	//pParticleManager = new ParticleManager();
	SoundEngine = createIrrKlangDevice();

	// Initialize SDL
	if((error = SDL_Init( SDL_INIT_VIDEO )) < 0 )
	{
		printf("Couldn't initialize SDL, error %i\n", error);
		return 1;
	}

	pWindow = SDL_CreateWindow("SDL2 window",		// window title
		SDL_WINDOWPOS_UNDEFINED,					// initial x position
		SDL_WINDOWPOS_UNDEFINED,					// initial y position
		width,										// width, in pixels
		height,										// height, in pixels
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	if (NULL == pWindow)
	{
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}
	
	SDL_GLContext context = SDL_GL_CreateContext(pWindow);

	glewInit();
	if (!GLEW_VERSION_2_0) {
		std::cout << "needs OpenGL version 2.0 or better" << std::endl;
		return 1;
	}

	SDL_GL_MakeCurrent(pWindow, context);
	glViewport(0, 0, width, height);
	
	pResourceManager = new ResourceManager();
	pTextRenderer = new TextRenderer();

	pObjectFactory->LoadLevelMeta("LevelMeta.json");

	unsigned int stopStep = 0;

	//SoundEngine->play2D("../Resources/Audio/breakout.mp3", GL_TRUE);
	// GAME LOOP
	while (true == appIsRunning)
	{
		if (!isGamePaused || !isGameStopped)
			frameTime = FRC->getFrameTime() / 1000.0f;
		else if(isGamePaused || isGameStopped)
			frameTime = 0.0166f;

		FRC->frameStart();
		pInputManager->update();

		if (!isGameStopped) {
			stopStep = 0;
			if (pInputManager->isTriggerred(SDL_SCANCODE_P)) {
				if (!isGamePaused)
					isGamePaused = true;
				if (isGamePaused)
					stopStep = 1;
			}
			if (pInputManager->isTriggerred(SDL_SCANCODE_S)) {
				if (isGamePaused)
					isGamePaused = false;
			}
			if (pInputManager->isTriggerred(SDL_SCANCODE_D)) {
				debugMode = !debugMode;
			}
			if (pInputManager->isTriggerred(SDL_SCANCODE_ESCAPE)) {
				if (isGamePaused) {
					pObjectFactory->destroyPause();
					isGamePaused = false;
					continue;
				}
			}
		}

		unsigned int count = pGameObjectManager->mGameObjects.size();
		for (unsigned int i = 0; i < count; ++i) {
			if (pGameObjectManager->mGameObjects[i]->mFlag) {
				pGameObjectManager->mGameObjects[i]->update();
			}
		}

		pEventManager->update(frameTime);

		pGameStateManager->update();

		pObjectFactory->update();

		if (!isGameStopped) {
			if (!isGamePaused || 1 == stopStep)
			{
				if (pInputManager->isTriggerred(SDL_SCANCODE_ESCAPE) && pObjectFactory->currentLevel != "Home.json") {
					if (!isGamePaused) {
						pObjectFactory->LoadLevel("PauseMenu.json");
						isGamePaused = true;
					}
				}

				// UPDATES
				pPhysicsManager->update(frameTime);

			}
		}

		pRenderer->update();

		//pTextRenderer->RenderText("LOUSY METEOR!", 25.0f, 25.0f, 1.0f, glm::vec3(0.5f, 0.5f, 0.5f));

		SDL_GL_SwapWindow(pWindow);

		FRC->frameEnd();
		
		Uint32 x = 1000 / FRC->getFrameTime();
		
		char textToWrite[4];
		sprintf_s(textToWrite, "%lu", x);
		SDL_SetWindowTitle(pWindow, textToWrite);
	}

	// Close and destroy the window
	SDL_DestroyWindow(pWindow);

	delete pInputManager;
	delete FRC;
	delete pResourceManager;
	delete pGameObjectManager;
	delete pObjectFactory;
	delete pPhysicsManager;
	delete pCollisionManager;
	delete pRenderer;
	delete pEventManager;
	delete pGameStateManager;

	// Quit SDL subsystems
	SDL_Quit();
	
	return 0;
}