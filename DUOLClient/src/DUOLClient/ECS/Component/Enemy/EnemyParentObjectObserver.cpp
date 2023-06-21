#include "DUOLClient/ECS/Component/Enemy/EnemyParentObjectObserver.h"
#include <rttr/registration>

#include "DUOLClient/Manager/EnemyManager.h"
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/GameObject.h"

using namespace rttr;

RTTR_REGISTRATION
{
rttr::registration::class_<DUOLClient::EnemyParentObjectObserver>("EnemyParentObjectObserver")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);

}

namespace DUOLClient
{
	EnemyParentObjectObserver::EnemyParentObjectObserver(DUOLGameEngine::GameObject* owner,
		const DUOLCommon::tstring& name) : DUOLGameEngine::MonoBehaviourBase(owner, name),
		_enemyOwner(nullptr), _enemyManager(nullptr)
	{
	}

	void EnemyParentObjectObserver::OnStart()
	{
		_enemyManager = EnemyManager::GetInstance();
	}

	void EnemyParentObjectObserver::OnCollisionStay(const std::shared_ptr<DUOLPhysics::Collision>& collision)
	{
		DUOLGameEngine::GameObject* otherGameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(collision->_other);

		if (otherGameObject->GetLayer() == TEXT("Enemy"))
		{
			auto other = _enemyManager->GetEnemy(otherGameObject);

			if (!other || other == _enemyOwner) return;

			other->PushedOut(GetTransform()->GetWorldPosition());
		}
	}
}
