#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/ECS/Object/AudioClip.h"
#include "DUOLGameEngine/Util/Coroutine/CoroutineHandler.h"

namespace DUOLGameEngine
{
	class Animator;
}

/**
 * \brief 등장씬을 위한 EliteMonster Script입니다.
 */
namespace DUOLClient
{
	class EliteMonsterScript : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		EliteMonsterScript(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("EliteMonsterScript"));

		virtual ~EliteMonsterScript() override;

	public:
		/**
 * \brief 씬이 시작할 때 OnStart 보다 이전에 호출됩니다.
 * 씬 시작 시 우선 순위의 게임 로직을 적용할 수 있습니다.
 */
		virtual void OnAwake();

		/**
		 * \brief 씬이 시작할 때 OnAwake 이후에 호출됩니다.
		 * 씬 시작 시 우선 순위의 게임 로직을 적용할 수 있습니다.
		 */
		virtual void OnStart();

		/**
		 * \brief 컴포넌트가 파괴될 때 호출됩니다.
		 */
		virtual void OnDestroy();

		/**
		 * \brief 매 프레임에 호출됩니다.
		 * \param deltaTime 프레임 간 시간 간격입니다.
		 */
		virtual void OnUpdate(float deltaTime);

		void EliteBossAnimateRun();

		void PlayerAnimateRun();

		void PlayStart();

	public:
		/**
		 * \brief EliteInfo
		 */
		float _startHeight;

		float _jumpAttackTime;

		float _jumpWaitTime;

		float _roarStartTime;

		float _roarEndTime;

		float _generationTime;

		/**
		 * \brief PlayerInfo
		 */
		DUOLMath::Vector3 _playerLocalPosition;

		float _playerAnimStart;

		float _playerAnimEnd;

		float _playerGenerateTime;

		bool _isAnimPlayed;

		bool _isJumping;
	private:
		DUOLGameEngine::CoroutineHandler EliteBossGenerate();

		DUOLGameEngine::CoroutineHandler PlayerGenerate();

	private:
		DUOLGameEngine::GameObject* _cachedEliteMonsterDummy;

		DUOLGameEngine::Animator* _cachedEliteMonsterAnimator;

		DUOLGameEngine::AudioClip* _roarClip;

		DUOLGameEngine::AudioClip* _groundAttack;

		DUOLGameEngine::AudioSource* _audioSource;

		DUOLGameEngine::GameObject* _cachedPlayerDummy;

		DUOLGameEngine::Animator* _cachedPlayerAnimator;


		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
		RTTR_REGISTRATION_FRIEND
	};
}