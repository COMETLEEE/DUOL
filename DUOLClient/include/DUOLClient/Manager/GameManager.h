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
	 * \brief ������ ��Ʈ�� �ϱ� ���� �޽����� ����
	 */
	enum class GameMessageType
	{
		SCENE_CHANGE					// �� ���� (parameter : tstring (�� �̸�))
		, BULLET_TIME_ALL				// ��� ��� �Ҹ� Ÿ��
		, BULLET_TIME_PLAYER			// �÷��̾� ���� �Ҹ� Ÿ��
	};

	/**
 * \brief ������ ��Ʈ�� �ϱ� ���� �޽����� ����
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
	 * \brief ������ ��Ʈ�� �ϱ� ���� �޽���
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
	 * \brief ���� �Ŵ���. �� �̵��� ���� �÷��̾��� ���� ���� ����, �ð�, ȯ�漳�� ���� ����Ѵ�.
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
		 * \brief ù ��° OnStart ������ �� ��ȯ ��û �Ҹ����� �ϱ� ���ؼ�.
		 */
		bool _isFirstStart;

		/**
		 * \brief Ŀ���� ���̳���. Display Count 0 ������� �ݺ��� API ȣ���ϴ� �ͺ��ٴ� �̰� �����ϰ� ���� �� ����.
		 */
		bool _isCursorShowing;

		/**
		 * \brief UI Mode�� ���� ��, �´� Ÿ�� �����Ϸ� ������ �ϴϱ� ����.
		 */
		float _timeScalePrevUIMode;

		/**
		 * \brief UI ���� ���� �� Game Mode �Դϴ�.
		 */
		GameMode _gameModePrevUIMode;

		/**
		 * \brief ���� ���� ����Դϴ�.
		 */
		GameMode _currentGameMode;

		/**
		 * \brief ���� Pause�� �� �ִ°�?.
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
		 * \brief �÷��̾��� ���� �����͸� ���� ���� �÷��̾ �����մϴ�. (OnStart ��� ..)
		 */
		void SetStartPlayerData();

		/**
		 * \brief �� ���� �� �÷��̾� ���õ� ������ ������ �ʿ䰡 ���� �� �����մϴ�.
		 */
		void SaveEndPlayerData();

		/**
		 * \brief �� ���� �����մϴ�. ������ �׻� Fade In Out�� �Բ�.
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
		 * \brief ���̵� ���� �����մϴ�. �ʹ� ȭ�鿡 ��ũ�� ���߱� ���� �ð��� ���� ������
		 * ���� ���� �����մϴ�.
		 * \return �ڷ�ƾ �ڵ鷯
		 */
		DUOLGameEngine::CoroutineHandler StartFadeIn();
		

#pragma region �� SCENE INITIALIZE
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
