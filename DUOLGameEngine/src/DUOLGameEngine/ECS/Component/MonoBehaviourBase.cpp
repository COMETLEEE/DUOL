#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGameEngine
{
	MonoBehaviourBase::MonoBehaviourBase(std::shared_ptr<DUOLGameEngine::GameObject> owner, const tstring& name) :
		BehaviourBase(std::move(owner), name)
	{
	}

	MonoBehaviourBase::~MonoBehaviourBase()
	{
	}
}