#pragma once

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Sprite.h
Purpose: Sprite Class
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "Component.h"
#include "SDL.h"
#include "..\Mesh.h"
#include "..\Shader.h"

class TextureOffset {
public:
	float textureOffsetX;
	float textureOffsetY;
	float currentTime;
	float timeLimit;
	float sheetDimensionX;
	float sheetDimensionY;

	TextureOffset() : textureOffsetX(0.0f), textureOffsetY(0.0f), currentTime(0.0f), timeLimit(100.0f),
						sheetDimensionX(1.0f), sheetDimensionY(1.0f)
	{}
};

class Sprite : public Component
{
public:
	Sprite();
	~Sprite();

	void update();
	void Serialize(GenericObject<false, Value::ValueType>);
	float *color;
	bool isAnim;
	bool hasAnim;
	TextureOffset* textureOffset;
	TextureOffset* animeTextureOffset;
	TextureOffset* currentTextureOffset;
	unsigned int mpMesh;

	inline unsigned int getMesh() { return mpMesh; };
	inline int getTextureID() { return mTextureID; }
	inline unsigned int getShaderID() { return shaderID; }
	float textureOffsetX();
	float textureOffsetY();

	float sheetDimensionX();
	float sheetDimensionY();
	unsigned int shaderID;
public:
	int mTextureID;
	unsigned int mAnimeID;
	std::string name;
};