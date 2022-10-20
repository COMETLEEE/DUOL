#include "DUOLGameEngine/ECS/Component/ColliderBase.h"

namespace DUOLGameEngine
{
	ColliderBase::ColliderBase(std::shared_ptr<DUOLGameEngine::GameObject> owner, const DUOLCommon::tstring& name) :
		BehaviourBase(std::move(owner), name)	// 일단 한 번의 복사밖에 없기 때문에 std::move 연산
	{
		 
	}

	ColliderBase::~ColliderBase()
	{

	}
}