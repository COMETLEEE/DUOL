#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "Enemy/EnemyGroupController.h"

namespace DUOLGameEngine
{
	class FadeInOut;
}

namespace DUOLClient
{
	class BossMonsterScript;

	class MainCameraController;

	class BossClearCheck : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		BossClearCheck(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("BossClearCheck"));

		virtual ~BossClearCheck() override;

	public:
		void OnAwake() override;

		void OnUpdate(float deltaTime) override;

		DUOLGameEngine::CoroutineHandler ScripteRun();

	private:
		DUOLGameEngine::GameObject* _bossMonsterController;

		EnemyGroupController* _bossMonsterControllerComp;

		DUOLGameEngine::FadeInOut* _fadeInOut;

		bool _isPlay;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
			RTTR_REGISTRATION_FRIEND
	};


}
