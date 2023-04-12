#include "DUOLClient/ECS/Component/Enemy/EnemySpawnTrigger.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyGroupController.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"

#include <rttr/registration>

#include "DUOLClient/Manager/EnemyManager.h"
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/GameObject.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::EnemySpawnTrigger>("EnemySpawnTrigger")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_enemyGroupControllerName", &DUOLClient::EnemySpawnTrigger::_enemyGroupControllerName)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::String)
	);


}

namespace DUOLClient
{
	EnemySpawnTrigger::EnemySpawnTrigger(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name),
		_enemyGroupController(nullptr), _enemyGroupControllerName(TEXT(""))
	{
	}

	EnemySpawnTrigger::~EnemySpawnTrigger()
	{
	}

	void EnemySpawnTrigger::OnStart()
	{
		const auto collider = GetGameObject()->GetComponent<DUOLGameEngine::ColliderBase>();

		if (!collider)
			GetGameObject()->AddComponent<DUOLGameEngine::BoxCollider>();

		const auto parent = GetGameObject()->GetTransform()->GetParent();

		if (parent)
			_enemyGroupController = parent->GetGameObject()->GetComponent<EnemyGroupController>();

		if (!_enemyGroupController)
			_enemyGroupController = EnemyManager::GetInstance()->GetEnemyGroupController(_enemyGroupControllerName);
	}

	void EnemySpawnTrigger::OnUpdate(float deltaTime)
	{
		MonoBehaviourBase::OnUpdate(deltaTime);
	}

	void EnemySpawnTrigger::OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
	{
		auto other = static_cast<DUOLGameEngine::GameObject*>(trigger->_other);

		if (other->GetTag() == TEXT("Player"))
		{
			_enemyGroupController->CreateEnemy();
			Destroy(GetGameObject());
		}
	}
}
