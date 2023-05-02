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
	GameManager::GameManager(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name)
		, _testTime(0.f)
		, _isFirst(true)
	{
		_firstSceneName = TEXT("Asset/Scene/Middle.dscene");

		_secondSceneName = TEXT("Asset/Scene/BattleTest.dscene");
	}

	GameManager::~GameManager()
	{

	}

	void GameManager::OnAwake()
	{
		MonoBehaviourBase::OnAwake();

		// 같은 객체가 있을 수 있으니, 하나만 존재해야하는 오브젝트를 만드는
		// Initialize Scene 을 통해서만 생성합니다.
		DontDestroyOnLoad(static_cast<DUOLGameEngine::GameObject*>(GetGameObject()));
	}

	void GameManager::OnUpdate(float deltaTime)
	{
		_testTime += deltaTime;

		if (_testTime > 20.f)
		{
			_isFirst
				? DUOLGameEngine::SceneManager::GetInstance()->LoadSceneFileFrom(_firstSceneName)
				: DUOLGameEngine::SceneManager::GetInstance()->LoadSceneFileFrom(_secondSceneName);

			_testTime = 0.f;

			_isFirst = !_isFirst;
		}
	}
}