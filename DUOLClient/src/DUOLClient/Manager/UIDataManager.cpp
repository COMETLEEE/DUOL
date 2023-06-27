#include "DUOLClient/Manager/UIDataManager.h"

#include <rttr/registration>

#include "DUOLGameEngine/Manager/UIManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Image.h"

namespace  DUOLClient
{
	DUOLClient::UIDataManager* DUOLClient::UIDataManager::_instance = nullptr;

	int DUOLClient::UIDataManager::_koCount;

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
		, _comboTime(0.f)
	{
	}

	DUOLClient::UIDataManager::~UIDataManager()
	{
	}

	void DUOLClient::UIDataManager::OnAwake()
	{
		if (!_instance)
		{
			_instance = this;

		}
	}

	void DUOLClient::UIDataManager::OnStart()
	{
		//Main Scene
	}

	void DUOLClient::UIDataManager::OnUpdate(float deltaTime)
	{
		_comboTime += deltaTime;

		if (7.0f <= _comboTime)
			KOUIActive(false);
		else
			KOUIActive(true);

	}

	void UIDataManager::ChangeScene()
	{

	}

	void DUOLClient::UIDataManager::SetPlayerHPUI(float hp)
	{
		DUOLGameEngine::UIManager::GetInstance()->SetScrollBarUI("HPBar", hp);
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

	void UIDataManager::KOUIActive(bool value)
	{
		DUOLGameEngine::UIManager::GetInstance()->SetActiveImage("Combo", value);
		DUOLGameEngine::UIManager::GetInstance()->SetActiveText("ComboNum", value);
	}

	void UIDataManager::PlusKOCount()
	{
		_koCount++;
		DUOLGameEngine::UIManager::GetInstance()->SetText("ComboNum", std::to_string(_koCount));
		_comboTime = 0.f;
	}

	DUOLClient::UIDataManager* UIDataManager::GetInstance()
	{
		if (_instance == nullptr)
			_instance = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty()->AddComponent<UIDataManager>();

		return _instance;
	}

	void UIDataManager::InitializeMiddle(DUOLGameEngine::GameObject* player)
	{
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
