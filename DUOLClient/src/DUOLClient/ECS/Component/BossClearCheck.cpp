#include "DUOLClient/ECS/Component/BossClearCheck.h"

#include "DUOLClient/Manager/SystemManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/FadeInOut.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"

#include <rttr/registration>

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::BossClearCheck>("BossClearCheck")
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
	BossClearCheck::BossClearCheck(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name):
		MonoBehaviourBase(owner, name),
		_isPlay(false)
	{
	}

	BossClearCheck::~BossClearCheck()
	{
	}

	void BossClearCheck::OnAwake()
	{
		auto scene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

		auto& allGameObjects = scene->GetAllGameObjects();


		for (auto gameObject : allGameObjects)
		{
			if (gameObject->GetName() == TEXT("BossMonsterSpawnPoint"))
			{
				_bossMonsterController = gameObject;
				_bossMonsterControllerComp = _bossMonsterController->GetComponent<EnemyGroupController>();
			}
	
			if (gameObject->GetTag() == TEXT("Fade"))
			{
				_fadeInOut = gameObject->GetComponent<DUOLGameEngine::FadeInOut>();
			}
		}

	}

	void BossClearCheck::OnUpdate(float deltaTime)
	{
		if (_bossMonsterControllerComp->GetIsClearGroup() && !_isPlay)
		{
			StartCoroutine(&DUOLClient::BossClearCheck::ScripteRun);
			_isPlay = true;
		}
	}



	DUOLGameEngine::CoroutineHandler BossClearCheck::ScripteRun()
	{
		//todo boss script 
		//스크립트 시작
		//마구마구마구하십쇼
		//co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(1.f);

		//스크립트 다재생완료! 1초쉬자
		co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(1.f);

		_fadeInOut->StartFadeOut(2, []() {});
		co_yield nullptr;

		while (_fadeInOut->GetFadeMode() != DUOLGameEngine::FadeInOutMode::DONE)
		{
			if (_fadeInOut->GetFadeMode() == DUOLGameEngine::FadeInOutMode::DONE)
			{
				_fadeInOut->SetUIOption(true);
			}

			co_yield nullptr;
		}

		_fadeInOut->SetUIOption(true);
		co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(1.f);

		//todo Boss Clear!!!!!! 크레딧씬으로 가즈아
		SystemManager::GetInstance()->ClearBoss();;

		co_return;
	}
}
