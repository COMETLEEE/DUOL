#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	enum class GameMode
	{
		DEFAULT
		, IN_BULLET_TIME_ALL
		, IN_BULLET_TIME_PLAYER
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
	};
	
	/**
	 * \brief ���� �Ŵ���. �� �̵��� ���� �÷��̾��� ���� ���� ����, �ð�, ȯ�漳�� ���� ����Ѵ�.
	 */
	class GameManager : public DUOLGameEngine::MonoBehaviourBase, public DUOLGameEngine::SingletonBase<DUOLClient::GameManager>
	{
		DECLARE_SINGLETON(DUOLClient::GameManager)

		DELETE_COPY_MOVE(DUOLClient::GameManager)

	public:
		GameManager(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = TEXT("GameManager"));

		virtual ~GameManager() override;

		std::queue<GameMessage<float>> _floatMessages;

		std::queue<GameMessage<DUOLCommon::tstring>> _tstringMessages;

	private:
		/**
		 * \brief ���� ���� ����Դϴ�.
		 */
		GameMode _currentGameMode;

		/**
		 * \brief �÷��̾��� ���� �����͸� ���� ���� �÷��̾ �����մϴ�. (OnStart ��� ..)
		 */
		void SetStartPlayerData();

		/**
		 * \brief �� ���� �� �÷��̾� ���õ� ������ ������ �ʿ䰡 ���� �� �����մϴ�.
		 */
		void SaveEndPlayerData();

		/**
		 * \brief �� �����մϴ�.
		 */
		void ChangeScene(const GameMessage<DUOLCommon::tstring>& message);

	public:
		virtual void OnAwake() override;

		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		bool IsInBulletTimeAll() const;

		bool IsInBulletTimePlayer() const;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

		RTTR_REGISTRATION_FRIEND
	};
}