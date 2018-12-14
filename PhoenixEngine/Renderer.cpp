/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Renderer.h
Purpose: Renderer
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "Renderer.h"

#include "Managers/GameObjectManager.h"
#include "GameObject.h"
#include "Components/Component.h"
#include "Components/Sprite.h"
#include "Components/Transform.h"
#include "Components/Body.h"
#include "Components/Controller.h"
#include "TextRenderer.h"
#include "Managers/FrameRateController.h"
#include "Components/Text.h"
#include "Managers/ResourceManager.h"
#include "ObjectFactory.h"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <time.h>
extern GameObjectManager* pGameObjectManager;
extern int width;
extern int height;
extern TextRenderer* pTextRenderer;
extern FrameRateController* FRC;
extern bool debugMode;
extern ResourceManager* pResourceManager;
extern ObjectFactory* pObjectFactory;

Renderer::Renderer() : shaderID(0), VAO(0), textureID(0), textureOffsetX(0.0f), textureOffsetY(0.0f),
						sheetX(1.0), sheetY(1.0), pTr(nullptr), pSpr(nullptr)
{}

Renderer::~Renderer() {
	delete pTr;
	delete pSpr;
}

void Renderer::update()
{
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Body *playerBody = nullptr;
	if(nullptr != pGameObjectManager->mPlayer)
		playerBody = static_cast<Body*>(pGameObjectManager->mPlayer->GetComponent(BODY));
	if(nullptr!=playerBody && playerBody->meteorHitCalled)
		glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
	else
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (nullptr != pGameObjectManager->mPlayer) {
		if (pGameObjectManager->mPlayer->mFlag) {
			std::string str = "LIFE: ";
			str += std::to_string(playerBody->life);
			pTextRenderer->RenderText(str, 25.0f, 560.0f, 1.0f, glm::vec3(0.5f, 0.5f, 0.5f));

			str = "SCORE: ";
			str += std::to_string(pObjectFactory->meteorsDestroyed);
			pTextRenderer->RenderText(str, 100.0f, 560.0f, 1.0f, glm::vec3(0.5f, 0.5f, 0.5f));

			str = "Homing Missile: ";
			str += std::to_string(pObjectFactory->mNoOfHoming);
			pTextRenderer->RenderText(str, 220.0f, 560.0f, 1.0f, glm::vec3(0.5f, 0.5f, 0.5f));

			str = "Nuclear Missile: ";
			str += std::to_string(pObjectFactory->mNoOfNuclear);
			pTextRenderer->RenderText(str, 410.0f, 560.0f, 1.0f, glm::vec3(0.5f, 0.5f, 0.5f));
		}
	}

	unsigned int count = pGameObjectManager->mGameObjects.size();
	for (unsigned int i = 0; i < count; ++i) {
		if (true == pGameObjectManager->mGameObjects[i]->mFlag) {

			GameObject* go = pGameObjectManager->mGameObjects[i];

			pSpr = static_cast<Sprite*>(go->GetComponent(SPRITE));
			if (nullptr != pSpr) {
				shaderID = pSpr->getShaderID();
				VAO = pSpr->getMesh();
				textureID = pSpr->getTextureID();

				textureOffsetX = pSpr->textureOffsetX();
				textureOffsetY = pSpr->textureOffsetY();
				sheetX = pSpr->sheetDimensionX();
				sheetY = pSpr->sheetDimensionY();
			}

			pTr = static_cast<Transform*>(go->GetComponent(TRANSFORM));
			pBody = static_cast<Body*>(go->GetComponent(BODY));

			if (nullptr != pTr && nullptr != pSpr) {
				Draw();
				pTr = nullptr;
				pSpr = nullptr;
			}

			Text* pText = static_cast<Text*>(go->GetComponent(TEXT));
			if (nullptr != pText) {
				pText->drawText();
			}

			if (go->mType == ENEMYSHIP) {
				std::string lifeString = "ENEMY LIFE: ";
				lifeString += std::to_string(static_cast<Body*>(go->GetComponent(BODY))->life);
				pTextRenderer->RenderText(lifeString, 640.0f, 560.0f, 1.0f, glm::vec3(0.5f, 0.5f, 0.5f));
			}

		}
	}
}

void Renderer::Draw()
{
	float x = pTr->mPosX;
	float y = pTr->mPosY;
	float scaleX = pTr->scaleX;
	float scaleY = pTr->scaleY;
	float colorx = pSpr->color[0];
	float colory = pSpr->color[1];
	float colorz = pSpr->color[2];

	glm::mat4 scale, rotate, transform, projection;
	
	scale = glm::scale(scale, glm::vec3(scaleX, scaleY, 0.0f));
	rotate = glm::rotate(rotate, pTr->mAngle, glm::vec3(0,0,1));

	transform = glm::translate(transform, glm::vec3(x, y, 0.0f)) * rotate * scale;
	projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f);
	glm::vec3 ucolor = glm::vec3(colorx, colory, colorz);

	// get matrix's uniform location and set matrix
	glUseProgram(shaderID);

	//srand((unsigned int)time(NULL));

	//float rand1 = (float)(rand()%10);
	////std::cout << "rand"<<rand1<<std::endl;
	//if(rand1>5){
	//	glUniform1i(glGetUniformLocation(shaderID, "setRandom"), 1);
	//}
	//else {
	//	glUniform1i(glGetUniformLocation(shaderID, "setRandom"), 0);
	//}
	

	unsigned int transformLoc = glGetUniformLocation(shaderID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	unsigned int colorLoc = glGetUniformLocation(shaderID, "color");
	glUniform3fv(colorLoc, 1, glm::value_ptr(ucolor));

	unsigned int orthoLoc = glGetUniformLocation(shaderID, "orthographic");
	glUniformMatrix4fv(orthoLoc, 1, GL_FALSE, glm::value_ptr(projection));

	unsigned int sheetsizeX = glGetUniformLocation(shaderID, "sheetsizeX");
	glUniform1f(sheetsizeX, sheetX);
	unsigned int sheetsizeY = glGetUniformLocation(shaderID, "sheetsizeY");
	glUniform1f(sheetsizeY, sheetY);
	unsigned int offsetX = glGetUniformLocation(shaderID, "offsetX");
	glUniform1f(offsetX, textureOffsetX);
	unsigned int offsetY = glGetUniformLocation(shaderID, "offsetY");
	glUniform1f(offsetY, textureOffsetY);

	if (pTr->mpOwner->mType == PLAYER) {
		if(pBody->meteorHitCalled)	glUniform4f(99, 1.0f, 1.0f, 1.0f, 0.4f);
		else glUniform4f(99, 1.0f, 1.0f, 1.0f, 1.0f);
	}
	else
		glUniform4f(99, 1.0f,1.0f,1.0f,1.0f);

	if (pTr->mpOwner->mType == SHIELD && debugMode) {
		glBindVertexArray(pResourceManager->circleVAO);
		glDrawArrays(GL_LINE_LOOP, 0, 16);
	}

	glBindVertexArray(VAO);

	if (-1 == textureID) {
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	else if (textureID  > -1) {

		if (debugMode && pTr->mpOwner->mType != SHIELD) {
			glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
		}

		if (0 != textureID)
		{
			glUniform1i(glGetUniformLocation(shaderID, "isTexture"), (int)1);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureID);
		}
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		if (0 != textureID)
			glUniform1i(glGetUniformLocation(shaderID, "isTexture"), (int)0);
	}
	if (debugMode && nullptr != pBody && (pBody->mVelX != 0.0f || pBody->mVelY != 0.0f)) {
		float lineAngle = atan2f(pBody->mVelY, pBody->mVelX);
		
		glm::mat4 scale, rotate, transform;
		scale = glm::scale(scale, glm::vec3(100.0f, 100.0f, 0.0f));
		rotate = glm::rotate(rotate, lineAngle, glm::vec3(0, 0, 1));
		transform = glm::translate(transform, glm::vec3(x, y, 0.0f)) * rotate * scale;

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(pResourceManager->lineVAO);
		glDrawArrays(GL_LINE_STRIP, 0, 2);
	}
}