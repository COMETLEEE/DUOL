#include "DUOLClient/Manager/UIDataManager.h"

#include <rttr/registration>

#include "DUOLGameEngine/Manager/UIManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"

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

	void UIDataManager::LoadUIDataJson()
	{
	}

	void UIDataManager::ChangeScene()
	{
		_imageUIList.clear();
		_textUIList.clear();
		_buttonUIList.clear();

	}

	DUOLClient::UIDataManager* UIDataManager::GetInstance()
	{
		if (_instance == nullptr)
			_instance = new UIDataManager();

		return _instance;
	}

	void UIDataManager::InitializeMiddle()
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

	void UIDataManager::PlayerHPUI(float hp)
	{
	}

}
