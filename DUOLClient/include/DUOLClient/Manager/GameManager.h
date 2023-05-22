#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	class FadeInOut;

	constexpr float BULLET_TIME_SCALE = 0.5f;

	constexpr float SCENE_START_FADE_IN = 2.f;

	constexpr float SCENE_END_FADE_OUT = 2.f;

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

		DUOLClient::FadeInOut* _fadeInOut;

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
		 * \brief ���� Main Scene�ΰ���.
		 */
		bool _isMainScene;

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

		DUOLGameEngine::CoroutineHandler StartUIMode();

		/**
		 * \brief ���̵� ���� �����մϴ�. �ʹ� ȭ�鿡 ��ũ�� ���߱� ���� �ð��� ���� ������
		 * ���� ���� �����մϴ�.
		 * \return �ڷ�ƾ �ڵ鷯
		 */
		DUOLGameEngine::CoroutineHandler StartFadeIn();

	public:
		virtual void OnAwake() override;

		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		bool IsInBulletTimeAll() const;

		bool IsInBulletTimePlayer() const;

		bool IsInUIMode() const;

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