#include "DUOLClient/ECS/Component/BossZoneTrigger.h"


#include <rttr/registration>

#include "DUOLClient/ECS/Component/Enemy/EnemyGroupController.h"
#include "DUOLClient/Manager/SystemManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::BossZoneTrigger>("BossZoneTrigger")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}


DUOLClient::BossZoneTrigger::BossZoneTrigger(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
	MonoBehaviourBase(owner, name)
{
}

DUOLClient::BossZoneTrigger::~BossZoneTrigger()
{
}

void DUOLClient::BossZoneTrigger::OnAwake()
{
	auto allGameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

	for (auto gameObject : allGameObjects)
	{
		if (gameObject->GetName() == TEXT("CautionCollider"))
		{
			_cautionCollider = gameObject;
		}
	}

	_cautionCollider->SetIsActiveSelf(false);
}

void DUOLClient::BossZoneTrigger::OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
{
	DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(trigger->_other);

	if (gameObject->GetTag() == TEXT("Player"))
	{
		_cautionCollider->SetIsActiveSelf(true);
	}
}

void DUOLClient::BossZoneTrigger::OnUpdate(float deltaTime)
{

}
