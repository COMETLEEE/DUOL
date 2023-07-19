#include "DUOLClient/ECS/Component/BStageCameraColiision.h"
#include "DUOLGameEngine/ECS/GameObject.h"

#include <rttr/registration>

#include "DUOLClient/Manager/SystemManager.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::BStageCameraColiision>("BStageCameraColiision")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

DUOLClient::BStageCameraColiision::BStageCameraColiision(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
	MonoBehaviourBase(owner, name)

{
}

DUOLClient::BStageCameraColiision::~BStageCameraColiision()
{
}

void DUOLClient::BStageCameraColiision::OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
{
	DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(trigger->_other);

	if (gameObject->GetTag() == TEXT("Player"))
	{
		DUOL_INFO(DUOL_CONSOLE, "B Stage Camera Action");

		DUOLClient::SystemManager::GetInstance()->BStageCameraDoorAction();
	}
}
