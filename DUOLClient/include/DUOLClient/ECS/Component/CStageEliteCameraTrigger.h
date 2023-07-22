#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/Manager/CameraEventManager.h"

namespace DUOLGameEngine
{
	class FadeInOut;
}

namespace DUOLClient
{
	class EliteMonsterScript;
	class EnemyGroupController;

	class DUOL_CLIENT_API CStageEliteCameraTrigger : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		CStageEliteCameraTrigger(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("CStageEliteCameraTrigger"));

		virtual ~CStageEliteCameraTrigger() override;

	public:
		void OnAwake() override;

		void OnUpdate(float deltaTime) override;

		void OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) override;

		DUOLGameEngine::CoroutineHandler ScripteRun(DUOLGameEngine::GameObject* player);

	private:
		DUOLGameEngine::GameObject* _playerSpawnPoint;

		DUOLGameEngine::GameObject* _eliteBossMonsterController;

		DUOLGameEngine::GameObject* _eliteBossTrigger;

		DUOLClient::EnemyGroupController* _eliteBossMonsterControllerComp;

		DUOLGameEngine::FadeInOut* _fadeInOut;

		bool _isFadeOut;

		// CameraAction
		std::vector<UINT64> _sequenceCamera;

		MainCameraController* _mainCameraController;

		EliteMonsterScript* _eliteScript;

		bool _isPlay;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
		RTTR_REGISTRATION_FRIEND
	};

}
