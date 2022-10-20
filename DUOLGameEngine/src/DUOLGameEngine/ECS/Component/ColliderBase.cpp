#include "DUOLGameEngine/ECS/Component/ColliderBase.h"

namespace DUOLGameEngine
{
	ColliderBase::ColliderBase(std::shared_ptr<DUOLGameEngine::GameObject> owner, const DUOLCommon::tstring& name) :
		BehaviourBase(std::move(owner), name)	// �ϴ� �� ���� ����ۿ� ���� ������ std::move ����
	{
		 
	}

	ColliderBase::~ColliderBase()
	{

	}
}