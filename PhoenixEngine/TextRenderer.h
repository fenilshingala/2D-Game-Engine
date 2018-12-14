#pragma once

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: TextRenderer.h
Purpose: Text Renderer
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include <map>
#include <glm/glm.hpp>
#include "Shader.h"
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
	unsigned int TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Horizontal offset to advance to next glyph
};

class TextRenderer
{
public:
	TextRenderer();
	~TextRenderer();

	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

	Shader* shader;
	std::map<char, Character> Characters;
	unsigned int VAO, VBO;
};