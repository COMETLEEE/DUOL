#include "DUOLClient/Manager/UIDataManager.h"

#include <rttr/registration>

#include "DUOLGameEngine/Manager/UIManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Image.h"

namespace  DUOLClient
{
	DUOLClient::UIDataManager* DUOLClient::UIDataManager::_instance = nullptr;

	using namespace rttr;

	RTTR_REGISTRATION
	{
		rttr::registration::class_<DUOLClient::UIDataManager>("UIDataManager")
		.constructor()
		(
			rttr::policy::ctor::as_raw_ptr
		)
		.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
		(
			rttr::policy::ctor::as_raw_ptr
		);
	}

	DUOLClient::UIDataManager::UIDataManager(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name)
	{
	}

	DUOLClient::UIDataManager::~UIDataManager()
	{
	}

	void DUOLClient::UIDataManager::OnAwake()
	{
		MonoBehaviourBase::OnAwake();
	}

	void DUOLClient::UIDataManager::OnStart()
	{
		//Main Scene
	}

	void DUOLClient::UIDataManager::OnUpdate(float deltaTime)
	{
		MonoBehaviourBase::OnUpdate(deltaTime);
	}

	void UIDataManager::ChangeScene()
	{

	}

	void DUOLClient::UIDataManager::SetPlayerHPUI(float hp)
	{
		DUOLGameEngine::UIManager::GetInstance()->SetScrollBarUI("HPBar",hp);
	}

	void DUOLClient::UIDataManager::SetPlayerOverDriveUI(float overdrive)
	{
		DUOLGameEngine::UIManager::GetInstance()->SetScrollBarUI("OverDrive", overdrive);
	}

	void DUOLClient::UIDataManager::SetUltimateUI(float ultimate)
	{
		DUOLGameEngine::UIManager::GetInstance()->SetScrollBarUI("UltimateSkill", ultimate);
	}

	void UIDataManager::SetPlayerHPMaxUI(float hp)
	{
		DUOLGameEngine::UIManager::GetInstance()->SetScrollBarUIMaxGauge("HPBar", hp);
	}

	void UIDataManager::SetPlayerOverDriveMaxUI(float overdrive)
	{
		DUOLGameEngine::UIManager::GetInstance()->SetScrollBarUIMaxGauge("OverDrive", overdrive);
	}

	void UIDataManager::SetUltimateMaxUI(float ultimate)
	{
		DUOLGameEngine::UIManager::GetInstance()->SetScrollBarUIMaxGauge("UltimateSkill", ultimate);
	}

	DUOLClient::UIDataManager* UIDataManager::GetInstance()
	{
		if (_instance == nullptr)
			_instance = new UIDataManager();

		return _instance;
	}

	void UIDataManager::InitializeMiddle(DUOLGameEngine::GameObject* player)
	{
		int a = 0;
	}

	void UIDataManager::InitializeStageA()
	{
	}

	void UIDataManager::InitializeStageB()
	{
	}

	void UIDataManager::InitializeStageC()
	{
	}

}
