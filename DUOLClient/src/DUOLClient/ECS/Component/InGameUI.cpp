#include "DUOLClient/ECS/Component/InGameUI.h"

#include "DUOLClient/Manager/GameManager.h"
#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/GameObject.h"

#include <rttr/registration>


using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::InGameUI>("InGameUI")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.method("DefaultMode", &DUOLClient::InGameUI::DefaultMode)
	.method("BulletTimePlayerMode", &DUOLClient::InGameUI::BulletTimePlayerMode)
	.method("BuolletTimeAllMode", &DUOLClient::InGameUI::BulletTimeAllMode)
	.method("UIMode", &DUOLClient::InGameUI::UIMode);
}

namespace DUOLClient
{
	InGameUI::InGameUI(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name)
	{
	}
	
	InGameUI::~InGameUI()
	{
	}

	void InGameUI::OnUpdate(float deltaTime)
	{
		MonoBehaviourBase::OnUpdate(deltaTime);
	}

	void InGameUI::UIMode()
	{
		bool ischeck = DUOLClient::GameManager::GetInstance()->IsInUIMode();

		if (ischeck == false)
			DUOL_INFO(DUOL_FILE, "UI Mode Is No Apply");
	}

	void InGameUI::BulletTimeAllMode()
	{
		bool ischeck=DUOLClient::GameManager::GetInstance()->IsInBulletTimeAll();

		if (ischeck == false)
			DUOL_INFO(DUOL_FILE, "BulletTimeAllMode Is No Apply");
	}

	void InGameUI::BulletTimePlayerMode()
	{
		bool ischeck = DUOLClient::GameManager::GetInstance()->IsInBulletTimePlayer();

		if (ischeck == false)
			DUOL_INFO(DUOL_FILE, "BulletTimePlayer Is No Apply");
	}

	void InGameUI::DefaultMode()
	{
		//bool ischeck = DUOLClient::GameManager::GetInstance()->IsDefaultMode();

		//if (ischeck == false)
		//	DUOL_INFO(DUOL_FILE, "Default Mode Is No Apply");
	}
}
