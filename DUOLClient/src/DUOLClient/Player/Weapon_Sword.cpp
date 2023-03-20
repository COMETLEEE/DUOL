#include "DUOLClient/Player/Weapon_Sword.h"

#include "DUOLGameEngine/ECS/GameObject.h"

#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::Weapon_Sword>("Weapon_Sword")
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
	Weapon_Sword::Weapon_Sword(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name)
	{
	}

	Weapon_Sword::~Weapon_Sword()
	{
	}

	void Weapon_Sword::OnStart()
	{
		MonoBehaviourBase::OnStart();
	}

	void Weapon_Sword::OnUpdate(float deltaTime)
	{
		// »Ï ..	
	}

	void Weapon_Sword::OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
	{
		MonoBehaviourBase::OnTriggerEnter(trigger);
	}

	void Weapon_Sword::OnTriggerStay(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
	{
		MonoBehaviourBase::OnTriggerStay(trigger);
	}

	void Weapon_Sword::OnTriggerExit(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
	{
		MonoBehaviourBase::OnTriggerExit(trigger);
	}
}