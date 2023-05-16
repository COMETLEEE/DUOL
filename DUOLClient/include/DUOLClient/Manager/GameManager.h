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
	 * \brief 게임을 컨트롤 하기 위한 메시지의 유형
	 */
	enum class GameMessageType
	{
		SCENE_CHANGE					// 씬 변경 (parameter : tstring (씬 이름))
		, BULLET_TIME_ALL				// 모든 요소 불릿 타임
		, BULLET_TIME_PLAYER			// 플레이어 빼고 불릿 타임
	};

	/**
	 * \brief 게임을 컨트롤 하기 위한 메시지
	 */
	template <typename TParam>
	struct GameMessage
	{
		GameMessageType _messageType;

		TParam _parameter;
	};
	
	/**
	 * \brief 게임 매니저. 씬 이동에 따른 플레이어의 현재 상태 저장, 시간, 환경설정 등을 담당한다.
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
		 * \brief 현재 게임 모드입니다.
		 */
		GameMode _currentGameMode;

		/**
		 * \brief 플레이어의 이전 데이터를 현재 씬의 플레이어에 적용합니다. (OnStart 등에서 ..)
		 */
		void SetStartPlayerData();

		/**
		 * \brief 씬 변동 등 플레이어 관련된 정보를 저장할 필요가 있을 때 저장합니다.
		 */
		void SaveEndPlayerData();

		/**
		 * \brief 씬 변동합니다.
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