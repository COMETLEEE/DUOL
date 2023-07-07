#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGameEngine
{
	class FadeInOut;
	class AudioListener;
	class AudioClip;
	class AudioSource;
	class Scene;
}

namespace DUOLClient
{

	constexpr float BULLET_TIME_SCALE = 0.5f;

	constexpr float SCENE_START_FADE_IN = 2.f;

	constexpr float SCENE_END_FADE_OUT = 2.f;

	constexpr DUOLMath::Vector3 MIDDLE_PORTAL_TO_A_POSITION = DUOLMath::Vector3(26.92f, 0.f, 14.1f);

	constexpr DUOLMath::Vector3 MIDDLE_PORTAL_TO_B_POSITION = DUOLMath::Vector3(24.1f, 0.f, 34.48f);

	constexpr DUOLMath::Vector3 MIDDLE_PORTAL_TO_C_POSITION = DUOLMath::Vector3(6.05f, 0.f, 7.6f);

	constexpr DUOLMath::Vector3 A_PORTAL_TO_MIDDLE_POSITION = DUOLMath::Vector3(1.65f, 3.77f, -0.083f);

	constexpr DUOLMath::Vector3 B_PORTAL_TO_MIDDLE_POSITION = DUOLMath::Vector3(8.12f, 4.0f, 61.07f);

	constexpr DUOLMath::Vector3 C_PORTAL_TO_MIDDLE_POSITION = DUOLMath::Vector3(-25.739f, 0.2f, -5.499f);

	enum class GameMode
	{
		DEFAULT
		, IN_BULLET_TIME_ALL
		, IN_BULLET_TIME_PLAYER
		, UI_MODE
	};

	/**
	 * \brief 게임을 컨트롤 하기 위한 메시지의 유형
	 */
	enum class GameMessageType
	{
		SCENE_CHANGE					// 씬 변경 (parameter : tstring (씬 이름))
		, BULLET_TIME_ALL				// 모든 요소 불릿 타임
		, BULLET_TIME_PLAYER			// 플레이어 빼고 불릿 타임
	};

	/**
 * \brief 게임을 컨트롤 하기 위한 메시지의 유형
 */
	enum class GameScene
	{
		Main	
		, Cut
		, Total
		, Middle
		, StageA
		, StageB
		, StageC
	};


	/**
	 * \brief 게임을 컨트롤 하기 위한 메시지
	 */
	template <typename TParam>
	struct GameMessage
	{
		GameMessageType _messageType;

		TParam _parameter;

		GameMessage() :
			_messageType()
			, _parameter()
		{

		}

		GameMessage(GameMessageType messageType, TParam&& param) :
			_messageType(messageType)
			, _parameter(param)
		{

		}

		GameMessage(const GameMessage&& other) noexcept :
			_messageType(other._messageType)
			, _parameter(other._parameter)
		{

		}
	};

	/**
	 * \brief 게임 매니저. 씬 이동에 따른 플레이어의 현재 상태 저장, 시간, 환경설정 등을 담당한다.
	 */
	class GameManager : public DUOLGameEngine::MonoBehaviourBase
	{
		DELETE_COPY_MOVE(DUOLClient::GameManager)

	public:
		GameManager(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("GameManager"));

		virtual ~GameManager() override;

		std::queue<GameMessage<float>> _floatMessages;

		std::queue<GameMessage<DUOLCommon::tstring>> _tstringMessages;

		static DUOLClient::GameManager* _instance;

		DUOLGameEngine::FadeInOut* _fadeInOut;
	private:
		/**
		 * \brief 첫 번째 OnStart 에서만 씬 전환 요청 불리도록 하기 위해서.
		 */
		bool _isFirstStart;

		/**
		 * \brief 커서가 보이나요. Display Count 0 만들려고 반복문 API 호출하는 것보다는 이게 안전하고 나을 것 같음.
		 */
		bool _isCursorShowing;

		/**
		 * \brief UI Mode가 끝날 때, 맞는 타임 스케일로 돌려야 하니까 저장.
		 */
		float _timeScalePrevUIMode;

		/**
		 * \brief UI 모드로 들어가기 전 Game Mode 입니다.
		 */
		GameMode _gameModePrevUIMode;

		/**
		 * \brief 현재 게임 모드입니다.
		 */
		GameMode _currentGameMode;

		/**
		 * \brief 현재 Pause할 수 있는가?.
		 */
		bool _canPausable;

		/**
		 * \brief Ingame UIMode Out flags.
		 */
		bool _isOutInGameUIMode;

		DUOLGameEngine::AudioSource* _bgmAudioSource;

		DUOLCommon::tstring _audioClipName;

		GameScene _currentGameScene;

		/**
		 * \brief 플레이어의 이전 데이터를 현재 씬의 플레이어에 적용합니다. (OnStart 등에서 ..)
		 */
		void SetStartPlayerData();

		/**
		 * \brief 씬 변동 등 플레이어 관련된 정보를 저장할 필요가 있을 때 저장합니다.
		 */
		void SaveEndPlayerData();

		/**
		 * \brief 씬 변동 예약합니다. 변동은 항상 Fade In Out과 함께.
		 */
		void ChangeScene(const GameMessage<DUOLCommon::tstring>& message);

		void MouseLock();

		void MouseUnLock();

		DUOLGameEngine::CoroutineHandler StartBulletTimeAll(float duration);

		DUOLGameEngine::CoroutineHandler StartBulletTimePlayer(float duration);

		void StartUIMode();

		void EndUIMode();

		void FadeOut();

		/**
		 * \brief 페이드 인을 실행합니다. 초반 화면에 싱크를 맞추기 위해 시간이 조금 지나고
		 * 실제 씬을 시작합니다.
		 * \return 코루틴 핸들러
		 */
		DUOLGameEngine::CoroutineHandler StartFadeIn();
		

#pragma region 각 SCENE INITIALIZE
		void CreatePortal(DUOLGameEngine::Scene* scene, const DUOLCommon::tstring& portalName, const DUOLCommon::tstring& nextSceneName, const DUOLMath::Vector3& position);;

		void InitializeMiddle(DUOLGameEngine::Scene* middle);

		void InitializeStageTotal();

		void InitializeStageA(DUOLGameEngine::Scene* stageA);

		void InitializeStageB(DUOLGameEngine::Scene* stageB);

		void InitializeStageC(DUOLGameEngine::Scene* stageC);
#pragma endregion

	public:
		virtual void OnAwake() override;

		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		bool IsInBulletTimeAll() const;

		bool IsInBulletTimePlayer() const;

		bool IsInUIMode() const;

		void SetIsOutInGameUIMode(bool value);

		void SetCanPausable(bool value);

		void SetBGM(DUOLGameEngine::AudioClip* audioClip);

		void SetBGM(DUOLCommon::tstring name);

		template <typename TParam>
		void PushGameMessage(GameMessage<TParam>&& message);

		static DUOLClient::GameManager* GetInstance();

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

		RTTR_REGISTRATION_FRIEND
	};

	template <typename TParam>
	void GameManager::PushGameMessage(GameMessage<TParam>&& message)
	{
		if constexpr (std::is_same_v<TParam, float>)
		{
			_floatMessages.push(std::move(message));
		}
		else if constexpr (std::is_same_v<TParam, DUOLCommon::tstring>)
		{
			_tstringMessages.push(std::move(message));
		}
	}
}
