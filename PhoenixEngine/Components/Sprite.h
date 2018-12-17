#pragma once

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