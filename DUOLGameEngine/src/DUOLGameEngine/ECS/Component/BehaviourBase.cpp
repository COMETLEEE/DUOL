#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"

namespace DUOLGameEngine
{
	BehaviourBase::BehaviourBase(std::shared_ptr<GameObject> owner, const tstring& name) :
		ComponentBase(owner, name),
		_isEnabled(true),
		_isActivedAndEnabled(false)
	{

	}

	BehaviourBase::~BehaviourBase()
	{

	}
}