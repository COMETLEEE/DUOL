#include "DUOLClient/ECS/Component/AStageAreaCameraTirgger.h"
#include "DUOLGameEngine/ECS/GameObject.h"

#include <rttr/registration>

#include "DUOLClient/Manager/SystemManager.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::AStageAreaCameraTirgger>("AStageAreaCameraTirgger")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

DUOLClient::AStageAreaCameraTirgger::AStageAreaCameraTirgger(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
	MonoBehaviourBase(owner, name)

{
}

DUOLClient::AStageAreaCameraTirgger::~AStageAreaCameraTirgger()
{
}

void DUOLClient::AStageAreaCameraTirgger::OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
{
	DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(trigger->_other);

	if (gameObject->GetTag() == TEXT("Player"))
	{
		DUOL_INFO(DUOL_CONSOLE, "A Stage Camera Action");

		DUOLClient::SystemManager::GetInstance()->AAreaCameraAction();
	}
}
