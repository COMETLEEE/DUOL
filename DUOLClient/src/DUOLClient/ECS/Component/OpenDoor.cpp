#include "DUOLClient/ECS/Component/OpenDoor.h"
#include "DUOLGameEngine/ECS/GameObject.h"

#include <rttr/registration>

#include "DUOLClient/Manager/SystemManager.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::OpenDoor>("OpenDoor")
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
	OpenDoor::OpenDoor(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		MonoBehaviourBase(owner, name)

	{
	}

	OpenDoor::~OpenDoor()
	{
	}

	void OpenDoor::OnCollisionStay(const std::shared_ptr<DUOLPhysics::Collision>& collision)
	{
		DUOLGameEngine::GameObject* otherGameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(collision->_other);

		if (otherGameObject->GetTag() == TEXT("Player") && SystemManager::GetInstance()->GetIsDoor())
		{
			DUOL_INFO(DUOL_CONSOLE, "OPEN DOOR");

			DUOLClient::SystemManager::GetInstance()->DoorCollisionStay();
		}
	}

	void OpenDoor::OnCollisionExit(const std::shared_ptr<DUOLPhysics::Collision>& collision)
	{
		DUOLGameEngine::GameObject* otherGameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(collision->_other);

		if (otherGameObject->GetTag() == TEXT("Player") && SystemManager::GetInstance()->GetIsDoor())
		{
			DUOLClient::SystemManager::GetInstance()->DoorCollisionStay();
		}
	}
}
