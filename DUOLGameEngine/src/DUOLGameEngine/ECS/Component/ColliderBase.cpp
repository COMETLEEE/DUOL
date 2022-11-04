#include "DUOLGameEngine/ECS/Component/ColliderBase.h"

namespace DUOLGameEngine
{
	ColliderBase::ColliderBase(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)	// �ϴ� �� ���� ����ۿ� ���� ������ std::move ����
	{
		 
	}

	ColliderBase::~ColliderBase()
	{

	}
}