#include "Controller.h"

#include "..\Managers\InputManager.h"
#include "..\Managers\FrameRateController.h"
#include "..\GameObject.h"
#include "Transform.h"
#include "Component.h"
#include "Body.h"
#include "../ObjectFactory.h"
#include "../Managers/GameStateManager.h"

#include "SDL.h"

extern InputManager* pInputManager;
extern FrameRateController* FRC;
extern ObjectFactory *pObjectFactory;
extern GameStateManager* pGameStateManager;
extern bool appIsRunning;

Controller::Controller() : Component(CONTROLLER) {}

Controller::~Controller() {}

void Controller::update()
{
	Body *pBody = static_cast<Body*>(mpOwner->GetComponent(BODY));
	Transform *pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));

	if (nullptr != pInputManager && nullptr != pBody) {

		if (pInputManager->isPressed(SDL_SCANCODE_UP)) 
			pBody->mTotalForceY += 100000.0;
		
		else if (pInputManager->isPressed(SDL_SCANCODE_DOWN)) 
			pBody->mTotalForceY -= 100000.0;
		
		else if (pInputManager->isPressed(SDL_SCANCODE_RIGHT)) 
			pBody->mTotalForceX += 100000.0;
		
		else if (pInputManager->isPressed(SDL_SCANCODE_LEFT)) 
			pBody->mTotalForceX -= 100000.0;
		
		else if (pInputManager->isButtonTriggerred(SDL_BUTTON_LEFT)) 
			pObjectFactory->createBullet(pBody->mPosX, pBody->mPosY, BULLET);
		
		else if (pInputManager->isTriggerred(SDL_SCANCODE_M)) 
			pObjectFactory->createObjectAtShip("HomingMissile.json");
		
		else if (pInputManager->isButtonTriggerred(SDL_BUTTON_RIGHT)) 
			pObjectFactory->createBullet(pBody->mPosX, pBody->mPosY, NUCLEARMISSILE);
		
	}
	
}