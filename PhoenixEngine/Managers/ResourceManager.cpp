/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ResourceManager.cpp
Purpose: Resource Manager
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "ResourceManager.h"
#include "..\Mesh.h"
#include "..\Shader.h"
#include "..\stb_image.h"

ResourceManager::ResourceManager() {
	shader1 = new Shader("../Resources/Shaders/Basic.shader");
	
	circleVAO = LoadCircle();
	if (circleVAO)
		mMeshes[CIRCLE] = circleVAO;

	lineVAO = LoadLine();
}

ResourceManager::~ResourceManager() {
	mMeshes.clear();
	mTextures.clear();
	delete shader1;
}

unsigned int ResourceManager::LoadMesh(unsigned int meshName) {

	//Check Hash map
	unsigned int pMesh = mMeshes[meshName];

	//Already Loaded?
	if (0 != pMesh)
		return pMesh;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
	if (RECTANGLE == meshName) {
		pMesh = LoadRect();
	}

	else if (TRIANGLE == meshName) {
		pMesh = LoadTriangle();
	}

	else if (CIRCLE == meshName) {
		pMesh = LoadCircle();
	}

	//Load Successfull? Add to hash map 
	if (pMesh)
		mMeshes[meshName] = pMesh;

	//reutrn to user
	return pMesh;
}

unsigned int ResourceManager::LoadRect() {
	float* v;
	int size;

	float vertices[] = {
		// positions          // texture coords
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f, // top right
		-0.5f,  -0.5f, 0.0f,   0.0f, 0.0f, // bottom right
		0.5f,  -0.5f, 0.0f,   1.0f, 0.0f, // bottom left
		0.5f,  0.5f, 0.0f,   1.0f, 1.0f  // top left
	};
	v = vertices;
	size = sizeof(vertices);
	
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 2,  // first Triangle
		2, 3, 0   // second Triangle
	};
	return ( new Mesh(shader1, v, size, false,
		indices, (unsigned int)(sizeof(indices))) )->getVAO();
}

unsigned int ResourceManager::LoadTriangle(unsigned int texture1) {
	float* v;
	int size;

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};
	v = vertices;
	size = sizeof(vertices);

	return (new Mesh(shader1, v, size, false))->getVAO();
}

unsigned int ResourceManager::LoadCircle() {
	float* v;
	int size;

	float vertices[] = {
			   0.5f,    0.0f,
			 0.433f,   0.25f,
			 0.353f,  0.353f,
			  0.25f,  0.433f,
			   0.0f,    0.5f,
			 -0.25f,  0.433f,
			-0.353f,  0.353f,
			-0.433f,   0.25f,
			  -0.5f,    0.0f,
			-0.433f,  -0.25f,
			-0.353f, -0.353f,
			 -0.25f, -0.433f,
			   0.0f,   -0.5f,
			  0.25f, -0.433f,
			 0.353f, -0.353f,
			 0.433f,  -0.25f
	};
	v = vertices;
	size = sizeof(vertices);

	return (new Mesh(shader1, v, size, true))->getVAO();
}

unsigned int ResourceManager::LoadLine() {
	float* v;
	int size;

	float vertices[] = {
		0.0f, 0.0f,
		0.5f, 0.0f
	};
	v = vertices;
	size = sizeof(vertices);

	return (new Mesh(shader1, v, size, true))->getVAO();
}

int ResourceManager:: getTextureID(const char* path) {
	
	auto itr = mTextures.find(path);
	//Already Loaded?
	if (itr != mTextures.end())
		return itr->second;
	
	unsigned int textureID;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (nrChannels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	shader1->useProgram(); 

	glUniform1i(glGetUniformLocation(shader1->getID(), "texture1"), 0);

	//if (textureID)
	mTextures[std::string(path)] = textureID;// = textureID;
	
	//std::cout << "resource manager - > "<<path<<std::endl;
	return (int)textureID;
	
}

unsigned int ResourceManager::getShaderID() { return shader1->getID(); }