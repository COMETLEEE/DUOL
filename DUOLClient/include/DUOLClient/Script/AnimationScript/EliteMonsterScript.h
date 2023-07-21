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

		DUOLGameEngine::CoroutineHandler AnimationStart();

		void PlayStart();

	public:
		float _startHeight;

		float _jumpAttackTime;

		float _jumpWaitTime;

		float _roarStartTime;

		float _roarEndTime;

		float _generationTime;


		bool _isAnimPlayed;

		bool _isJumping;

	private:
		DUOLGameEngine::GameObject* _cachedEliteMonsterDummy;

		DUOLGameEngine::Animator* _cachedEliteMonsterAnimator;

		DUOLGameEngine::AudioClip* _roarClip;

		DUOLGameEngine::AudioClip* _groundAttack;

		DUOLGameEngine::AudioSource* _audioSource;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
		RTTR_REGISTRATION_FRIEND
	};
}