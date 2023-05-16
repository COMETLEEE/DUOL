#include "DUOLClient/Manager/GameManager.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::GameManager>("GameManager")
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
	GameManager::GameManager() :
		DUOLGameEngine::MonoBehaviourBase(nullptr, TEXT("GameManager"))
	{
		
	}

	GameManager::GameManager(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name)
	{
		
	}

	GameManager::~GameManager()
	{

	}

	void GameManager::SetStartPlayerData()
	{
		// TODO
	}

	void GameManager::SaveEndPlayerData()
	{
		// TODO
	}

	void GameManager::ChangeScene(const GameMessage<DUOLCommon::tstring>& message)
	{
		// TODO
	}

	void GameManager::OnAwake()
	{
		MonoBehaviourBase::OnAwake();

		// 같은 객체가 있을 수 있으니, 하나만 존재해야하는 오브젝트를 만드는
		// Initialize Scene 을 통해서만 생성합니다.
		DontDestroyOnLoad(static_cast<DUOLGameEngine::GameObject*>(GetGameObject()));
	}

	void GameManager::OnStart()
	{
		MonoBehaviourBase::OnStart();

		DUOL_ENGINE_INFO(DUOL_CONSOLE, "GameManager 'OnStart' function called.")
	}

	void GameManager::OnUpdate(float deltaTime)
	{
		// 메세지들에 대해서 처리 ..
	}

	bool GameManager::IsInBulletTimeAll() const
	{
		return _currentGameMode == GameMode::IN_BULLET_TIME_ALL;
	}

	bool GameManager::IsInBulletTimePlayer() const
	{
		return _currentGameMode == GameMode::IN_BULLET_TIME_PLAYER;
	}
}