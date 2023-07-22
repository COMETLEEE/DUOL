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
 * \brief ������� ���� EliteMonster Script�Դϴ�.
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
 * \brief ���� ������ �� OnStart ���� ������ ȣ��˴ϴ�.
 * �� ���� �� �켱 ������ ���� ������ ������ �� �ֽ��ϴ�.
 */
		virtual void OnAwake();

		/**
		 * \brief ���� ������ �� OnAwake ���Ŀ� ȣ��˴ϴ�.
		 * �� ���� �� �켱 ������ ���� ������ ������ �� �ֽ��ϴ�.
		 */
		virtual void OnStart();

		/**
		 * \brief ������Ʈ�� �ı��� �� ȣ��˴ϴ�.
		 */
		virtual void OnDestroy();

		/**
		 * \brief �� �����ӿ� ȣ��˴ϴ�.
		 * \param deltaTime ������ �� �ð� �����Դϴ�.
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