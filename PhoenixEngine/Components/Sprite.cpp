/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Sprite.cpp
Purpose: Sprite Class
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "Sprite.h"
#include "..\Managers\ResourceManager.h"
#include "..\GameObject.h"
#include "Transform.h"
#include "../Managers/FrameRateController.h"
#include "../ObjectFactory.h"
#include <String>

extern FrameRateController* FRC;
extern ResourceManager* pResourceManager;
extern ObjectFactory *pObjectFactory;

Sprite::Sprite() : Component(SPRITE), color(new float[3]), isAnim(false), textureOffset(nullptr),
				 animeTextureOffset(nullptr), currentTextureOffset(nullptr), 
				 shaderID(0), mpMesh(0), mTextureID(0)
{}

Sprite::~Sprite() {
	delete[] color; delete textureOffset;
}

void Sprite::update() {
	if (isAnim) {

		Body *pBody = static_cast<Body*>(mpOwner->GetComponent(BODY));
		pBody->mVelX = 0.0f;
		pBody->mVelY = 0.0f;
		if (currentTextureOffset->currentTime > currentTextureOffset->timeLimit) {
			currentTextureOffset->currentTime = 0.0f;

			currentTextureOffset->textureOffsetX += currentTextureOffset->sheetDimensionX;
			if (currentTextureOffset->textureOffsetX == 1.0f)
			{
				currentTextureOffset->textureOffsetX = 0.0f;

				currentTextureOffset->textureOffsetY -= currentTextureOffset->sheetDimensionY;
				if (currentTextureOffset->textureOffsetY == 0.0f)
					pObjectFactory->deleteObjects.push_back(mpOwner);
			}
		}
		else
			currentTextureOffset->currentTime += FRC->getFrameTime();
	}
}

float Sprite::textureOffsetX() {
	if(nullptr != textureOffset)
		return currentTextureOffset->textureOffsetX;
	else return 0.0f;
}
float Sprite::textureOffsetY() {
	if (nullptr != textureOffset)
		return currentTextureOffset->textureOffsetY;
	else return 0.0f;
}

float Sprite::sheetDimensionX() { 
	if (nullptr != textureOffset)
		return currentTextureOffset->sheetDimensionX;
	else return 0.0f;
}
float Sprite::sheetDimensionY() {
	if (nullptr != textureOffset)
		return currentTextureOffset->sheetDimensionY;
	else return 0.0f;
}

void Sprite::Serialize(GenericObject<false, Value::ValueType> obj) {

	std::string shapeType = obj["Shape"].GetString();
	color[0] = obj["colorx"].GetFloat();
	color[1] = obj["colory"].GetFloat();
	color[2] = obj["colorz"].GetFloat();

	unsigned int meshType;
	std::string texturePath = "";
	if (obj.HasMember("texture")) {
		texturePath += obj["texture"].GetString();
		name = texturePath;
	}
	if ("Rectangle" == shapeType) {
		meshType = RECTANGLE;
	}
	else if ("Triangle" == shapeType) {
		meshType = TRIANGLE;
		mTextureID = -1;
	}
	else if ("Circle" == shapeType) {
		meshType = CIRCLE;
		mTextureID = -2;
	}

	if ("" != texturePath){
		mpMesh = pResourceManager->LoadMesh(meshType);
		mTextureID = pResourceManager->getTextureID(texturePath.c_str());
		textureOffset = new TextureOffset();
		currentTextureOffset = textureOffset;
		if (obj.HasMember("animation")) {
			hasAnim = true;
			std::string animePath = "";
			animePath += obj["animation"].GetString();
			mAnimeID = pResourceManager->getTextureID(animePath.c_str());
			animeTextureOffset = new TextureOffset();
			animeTextureOffset->sheetDimensionX = (1.0f / (float)obj["sheetdimensionX"].GetInt());
			animeTextureOffset->sheetDimensionY = (1.0f / (float)obj["sheetdimensionY"].GetInt());
			animeTextureOffset->textureOffsetY = ((float)obj["sheetdimensionY"].GetInt() - 1.0f) * animeTextureOffset->sheetDimensionY;
		}
	}
	else
		mpMesh = pResourceManager->LoadMesh(meshType);
	shaderID = pResourceManager->getShaderID();
}