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