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
	class InteractiveScript;
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

	constexpr DUOLMath::Vector3 A_CLEAR_PORTAL_TO_MIDDLE_POSITION = DUOLMath::Vector3(86.5, 7.86, -32);

	constexpr DUOLMath::Vector3 B_CLEAR_PORTAL_TO_MIDDLE_POSITION = DUOLMath::Vector3(54.7, 28.6, 71.71f);

	constexpr DUOLMath::Vector3 C_CLEAR_PORTAL_TO_MIDDLE_POSITION = DUOLMath::Vector3(-130, 7.5, 32);

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

		DUOLClient::InteractiveScript* _interactiveScript;

#pragma region function
	public:
		virtual void OnAwake() override;

		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		static DUOLClient::SystemManager* GetInstance();

		static void SetAstageClear(bool value);


		/**
		 * \brief ALL 
		 */
		void SetGameScene(GameScene gamescene) { _currentGameScene = gamescene; }

		void FirstMonsterActionSet() { _isFirstMonsterAction = true; }

		void PlayerCameraAction(std::string name, DUOLGameEngine::Transform* playertransform);

		bool GetFirstMonster() { return _isFirstMonster; }

		bool IsGameraSequenceMode() { return _isCameraSequenceMode; }

		bool GetIsEnemyAiPlay() { return _isEnemyAIPlay; }

		void CreatePortal(const DUOLCommon::tstring& portalName, const DUOLCommon::tstring& nextSceneName, const DUOLMath::Vector3& position);;


		/**
		 * \brief Total Scene
		 */
		void FinishTotalScene();


		/**
		 * \brief Middle Scene
		 */
		void SetMiddleScene() { _isMiddleEvent = true; }


		 /**
		  * \brief A Scene
		  */


		  /**
		 * \brief B Scene
		 */

		void SetBStageAllMonsterKill(bool value) { _isBStageAllMonsterKill = value; }

		inline bool GetIsDoor() { return _isDoorMonsterKill; }

		void BSystem(float deltaTime);

		void RimLightFromDeltaTime(float deltaTime);

		void OpenDoor(float deltaTime);

		void DoorCollisionStay();

		void CloseDoorUI();

		void BStageCameraDoorAction();

		void BStageCameraFactoryAction();

		void BStageCameraMonsterWaveAction();


		 /**
		* \brief C Scene
		*/



		/**
		* \brief Sound
		 */

		void PlayUISound(UISound uiindex, bool istrue);

		void PlayTotalScene(DUOLCommon::tstring sound);

		void PlayMiddleScene(DUOLCommon::tstring sound);

		void PlayStageBScene(DUOLCommon::tstring sound);


	private:
		/**
		* \brief Init
		*/
		void InitializeMiddle();

		void InitializeStageTotal();

		void InitializeStageA();

		void InitializeStageB();

		void InitializeStageC();

		void InitializeStage();


		/**
		* \brief UpDate
		*/
		void MiddleUpdate(float deltaTime);

		void StageBUpdate(float deltaTime);


		void BossUI();

		void PlaySound(DUOLGameEngine::AudioClip* soundClip);


#pragma region 

	private:
		/**
		 * \brief ALL
		*/
		float _currentTime;

		bool _isCameraSequenceMode;

		GameScene _currentGameScene;

		DUOLClient::Player* _player;

		DUOLGameEngine::FadeInOut* _fadeInOut;

		std::shared_ptr<DUOLGameEngine::CameraEventManager> _cameraManager;

		std::shared_ptr<DUOLGameEngine::SoundManager> _soundManager;

		static bool _isMiddleEvent;

		static bool _isFirstMonster;

		static bool _isAStageClear;

		static bool _isBStageClear;

		static bool _isCStageClear;

		int _scriptIndex;

		int _preScriptIndex;

		bool _isEnemyAIPlay;

		bool _isShowScript;

		bool _isShowInfo;
		// each stage first monster meet
		bool _isFirstMonsterAction;


		/**
		 * \brief Total Scene
		*/
		bool _isTotalSceneEnd;


		/**
		 * \brief Middle Scene
		*/
		bool _isStartScript;

		bool _isPlayerOverdrive;

		bool _isPlayerUltra;

		 /**
		* \brief A Scene
		 */


		 /**
		 * \brief B Scene
		 */
		DUOLGameEngine::GameObject* _dangerObject;

		DUOLGameEngine::GameObject* _dangerColliderObject;

		DUOLGameEngine::GameObject* _infoObject;

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

		bool _isBWaveScriptCheck;

		bool _isCreatePortal;

		 /**
		* \brief C Scene
		*/
		DUOLGameEngine::GameObject* _bossName;

		DUOLGameEngine::GameObject* _bossHPBar;

		

		/**
		* \brief Sound
		*/
		std::map<DUOLCommon::tstring,DUOLGameEngine::AudioClip*> _totalSceneClips;

		std::map<DUOLCommon::tstring,DUOLGameEngine::AudioClip*> _middleSceneClips;

		std::vector<DUOLGameEngine::AudioClip*> _aSceneClips;

		std::map<DUOLCommon::tstring,DUOLGameEngine::AudioClip*> _bSceneClips;

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


	private:
		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

		RTTR_REGISTRATION_FRIEND

	};

}