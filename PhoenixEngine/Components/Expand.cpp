#include "Expand.h"

#include "../Managers/PhysicsManager.h"
#include "../ObjectFactory.h"
#include "../GameObject.h"
#include "../Managers/EventManager.h"

extern EventManager* pEventManager;

Expand::Expand() : Component(EXPAND), collideCalled(false)
{
}


Expand::~Expand()
{
}

void Expand::update()
{
	Transform *pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
	if (collideCalled) {
		pTr->scaleX += 2.0f;
		pTr->scaleY += 2.0f;
	}
}

void Expand::Serialize(GenericObject<false, Value::ValueType>)
{}

void Expand::HandleEvent(Event *pEvent)
{
	if (COLLIDE == pEvent->mType)
	{
		if (false == collideCalled)
			collideCalled = true;
	}
}