#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/Util/Defines.h"

namespace DUOLGameEngine
{
	class FadeInOut;
	class GameObject;
}

namespace DUOLClient
{
	class Player;
	enum class GameScene;
	class MainCameraController;
}

namespace DUOLClient
{
	enum class DialogueTable
	{
		Dialogue04,
		Dialogue05,
		Dialogue06,
		Dialogue07,

		NONE
	};
}

namespace DUOLClient
{
	/**
	 * \brief system manager : 게임에 각 구역의 기믹을 관리하는 매니저 
	 */
	class SystemManager : public DUOLGameEngine::MonoBehaviourBase
	{
		DELETE_COPY_MOVE(DUOLClient::SystemManager)

	public:
		SystemManager(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("SystemManager"));

		virtual ~SystemManager() override;

	private:
		static DUOLClient::SystemManager* _instance;

		DUOLClient::MainCameraController* _mainCameraController;

	private:
		float _currentTime;
		/**
		* \brief Stage A
		 */


		/**
		* \brief Stage B
		*/
		DUOLGameEngine::GameObject* _rimLightObject;

		DUOLGameEngine::GameObject* _doorObject;

		bool _isBStageAllMonsterKill;

		bool _isDoorMonsterKill;

		bool _isOpenDoor;

		float _moveDoor;

		float _rimPower;


		/**
		* \brief Stage C
		*/


		bool _isCameraSequenceMode; 

		GameScene _currentGameScene;

		bool _isNextScript;

	public:
		virtual void OnAwake() override;

		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		void PlayerCameraAction(std::string name,DUOLGameEngine::Transform* playertransform);

		void SetGameScene(GameScene gamescene) { _currentGameScene = gamescene; }

		/**
		 * \brief Stage A
		 */

		/**
		 * \brief Stage B
		 */
		void SetBStageAllMonsterKill(bool value) { _isBStageAllMonsterKill = value; }

		inline bool GetIsDoor() { return _isDoorMonsterKill; }

		void BSystem(float deltaTime);

		void RimLightFromDeltaTime(float deltaTime);

		void OpenDoor(float deltaTime);

		void DoorCollisionStay();

		/**
		 * \brief Stage C
		 */

		static DUOLClient::SystemManager* GetInstance();

		bool IsGameraSequenceMode() { return _isCameraSequenceMode; }

	private:
		void InitializeMiddle();

		void InitializeStageTotal();

		void InitializeStageA();

		void InitializeStageB();

		void InitializeStageC();

		void MiddleUpdate(float deltaTime);

		void ChangeScript(DialogueTable dialogue);

		void ShowScript();

		void ShowInfoUI();

		void BossUI();

		void ScriptCheck(float deltaTime);

		std::vector<std::pair<DUOLCommon::tstring, float>> _scriptList;

		DUOLClient::Player* _player;

		DUOLGameEngine::FadeInOut* _fadeInOut;

		DUOLGameEngine::GameObject* _scriptObject;

		DUOLGameEngine::GameObject* _infoObject;

		DUOLGameEngine::GameObject* _bossName;

		DUOLGameEngine::GameObject* _bossHPBar;

		int _scriptIndex;

		float _scriptTime;

		float _infoTime;

	private:
		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

			RTTR_REGISTRATION_FRIEND

	};

}