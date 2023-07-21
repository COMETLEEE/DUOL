#include "DUOLClient/ECS/Component/CStageEliteCameraTrigger.h"
#include "DUOLGameEngine/ECS/GameObject.h"

#include <rttr/registration>

#include "DUOLClient/Manager/SystemManager.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::CStageEliteCameraTrigger>("CStageEliteCameraTrigger")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

DUOLClient::CStageEliteCameraTrigger::CStageEliteCameraTrigger(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
	MonoBehaviourBase(owner, name)
{
}

DUOLClient::CStageEliteCameraTrigger::~CStageEliteCameraTrigger()
{
}

void DUOLClient::CStageEliteCameraTrigger::OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
{
	DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(trigger->_other);

	if (gameObject->GetTag() == TEXT("Player"))
	{
		DUOL_INFO(DUOL_CONSOLE, "C Stage Elite Action");

		DUOLClient::SystemManager::GetInstance()->CStageEliteMonsterCameraAction();
	}
}
