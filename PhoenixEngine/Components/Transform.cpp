/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Transform.cpp
Purpose: Transform Class
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine - 2 - DataDriven
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "Transform.h"

Transform::Transform() : Component(TRANSFORM), mPosX(0.0f), mPosY(0.0f), scaleX(0.0f), scaleY(0.0f), mAngle(0.0f)
{}

void Transform::Serialize(GenericObject<false, Value::ValueType> obj) {
	if(obj.HasMember("posx"))
		mPosX = obj["posx"].GetFloat();
	if (obj.HasMember("posy"))
		mPosY = obj["posy"].GetFloat();
	if (obj.HasMember("scaleX"))
		scaleX = obj["scaleX"].GetFloat();
	if (obj.HasMember("scaleY"))
		scaleY = obj["scaleY"].GetFloat();
}