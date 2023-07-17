#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/Util/Defines.h"

namespace DUOLGameEngine
{
	class SoundManager;
	class AudioClip;
	class CameraEventManager;
	class FadeInOut;
	class GameObject;
	class AudioListener;
	class AudioSource;
	enum class FadeInOutMode;
}

namespace DUOLClient
{
	class Player;
	enum class GameScene;
	class MainCameraController;
}

namespace DUOLClient
{
	enum class UISound
	{
		ButtonClick,
		ButtonNoClick,
		WindowActive,
		WindowInActive,

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
		static DUOLClient::SystemManager* _systemInstance;

		std::shared_ptr<DUOLGameEngine::CameraEventManager> _cameraInstance;

		DUOLClient::MainCameraController* _mainCameraController;

	public:
		virtual void OnAwake() override;

		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		void FinishTotalScene();

		void FirstMonsterActionSet() { _isFirstMonsterAction = true; }

		void PlayerCameraAction(std::string name, DUOLGameEngine::Transform* playertransform);

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

		void CloseDoorUI();

		void BStageCameraFirstAction();

		void BStageCameraFactoryAction();

		void BStageCameraMonsterWaveAction();

		/**
		 * \brief Stage C
		 */

		static DUOLClient::SystemManager* GetInstance();

		bool IsGameraSequenceMode() { return _isCameraSequenceMode; }

		bool GetIsEnemyAiPlay() { return _isEnemyAIPlay; }

		void PlayUISound(UISound uiindex, bool istrue);

	private:
		void InitializeMiddle();

		void InitializeStageTotal();

		void InitializeStageA();

		void InitializeStageB();

		void InitializeStageC();

		void InitializeStage();

		void MiddleUpdate(float deltaTime);

		void StageBUpdate(float deltaTime);

		void ChangeScript(int index);

		void ShowScript();

		void ShowInfoUI();

		void BossUI();

		void ScriptCheck(float deltaTime, bool check = false);

		void Infocheck(float deltaTime,bool check=false);

		void PlaySound(DUOLGameEngine::AudioClip* soundClip);

	private:
		float _currentTime;

		float _currentScriptTime;

		float _currentInfoTime;

		// each stage first monster meet
		bool _isFirstMonsterAction;

		int _firstMonsterScriptCount;

		int _firstMonsterInfoCount;

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

		bool _isBStageDoorAction;

		bool _isBStageFactoryAction;

		bool _isBStageMonsterWaveAction;

		/**
		* \brief Stage C
		*/


		bool _isCameraSequenceMode;

		GameScene _currentGameScene;

		std::vector<std::tuple<DUOLCommon::tstring, float,bool>> _scriptList;

		std::vector<std::tuple<DUOLCommon::tstring, float, bool>> _infoList;

		std::vector<bool> _infoCheckList;

		DUOLClient::Player* _player;

		DUOLGameEngine::FadeInOut* _fadeInOut;

		DUOLGameEngine::GameObject* _scriptObject;

		DUOLGameEngine::GameObject* _infoObject;

		DUOLGameEngine::GameObject* _bossName;

		DUOLGameEngine::GameObject* _bossHPBar;

		std::shared_ptr<DUOLGameEngine::CameraEventManager> _cameraManager;

		std::shared_ptr<DUOLGameEngine::SoundManager> _soundManager;

		std::vector<DUOLGameEngine::AudioClip*> _firstMonsterClips;

		std::vector<DUOLGameEngine::AudioClip*> _totalSceneClips;

		std::vector<DUOLGameEngine::AudioClip*> _middleSceneClips;

		std::vector<DUOLGameEngine::AudioClip*> _aSceneClips;

		std::vector<DUOLGameEngine::AudioClip*> _bSceneClips;

		std::vector<DUOLGameEngine::AudioClip*> _cSceneClips;

		std::vector<DUOLGameEngine::AudioClip*> _uiClips;

		/**
		* \brief 사운드 컴포넌트 발생 / 오디오가 없을경우 발생하기 위해 넣어줌
		*/
		DUOLGameEngine::AudioSource* _audioSource;

		DUOLGameEngine::AudioSource* _uiAudioSource;

		/**
		* \brief 사운드 컴포넌트 듣는
		*/
		DUOLGameEngine::AudioListener* _audioListener;

		DUOLGameEngine::AudioSource* _auraSource;

		static bool _isMiddleEvent;

		static bool _isFirstMonster;

		static bool _isAStageClear;

		static bool _isBStageClear;

		static bool _isCStageClear;

		int _scriptIndex;

		int _infoIndex;

		float _scriptTime;

		float _infoTime;

		bool _isEnemyAIPlay;

		bool _isNextScript;

		bool _isNextInfo;

		bool _isShowScript;

		bool _isShowInfo;
	private:
		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

			RTTR_REGISTRATION_FRIEND

	};

}