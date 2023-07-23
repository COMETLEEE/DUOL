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
	/**
	 * \brief  계단 오르고 밟는 트리거
	 */
	class DUOL_CLIENT_API CStageBossCameraTrigger : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		CStageBossCameraTrigger(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("CStageBossCameraTrigger"));

		virtual ~CStageBossCameraTrigger() override;

	public:
		void OnAwake() override;

		void OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) override;

		DUOLGameEngine::CoroutineHandler ScripteRun(DUOLGameEngine::GameObject* player);

	private:
		DUOLGameEngine::GameObject* _bossMonsterController;

		EnemyGroupController* _bossMonsterControllerComp;

		DUOLGameEngine::GameObject* _bossTrigger;

		// CameraAction
		std::vector<UINT64> _sequenceCamera;

		MainCameraController* _mainCameraController;

		DUOLGameEngine::FadeInOut* _fadeInOut;

		BossMonsterScript* _bossScript;

		AudioSource

		bool _isPlay;

		bool _isFadeOut;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
		RTTR_REGISTRATION_FRIEND
	};
}
