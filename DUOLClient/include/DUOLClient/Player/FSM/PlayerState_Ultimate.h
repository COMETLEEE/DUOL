#pragma once
#include "DUOLClient/Player/FSM/PlayerStateBase.h"

namespace DUOLGameEngine
{
	class ParticleRenderer;
}

namespace DUOLClient
{
	class PlayerState_Ultimate : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Ultimate(DUOLClient::Player* player);

		virtual ~PlayerState_Ultimate();
	public:

	private:
		DUOLGameEngine::ParticleRenderer* _particleOverdrive;

		DUOLGameEngine::AnimatorController* _playerNormalAnimCon;

		DUOLGameEngine::AnimatorController* _playerOverdriveSwordAnimCon;

		DUOLGameEngine::AnimatorController* _playerOverdriveFistAnimCon;

		DUOLGameEngine::ParticleRenderer* _overdriveSwordTrail;

		DUOLGameEngine::ParticleRenderer* _leftFistFormAura;

		DUOLGameEngine::ParticleRenderer* _rightFistFormAura;

		bool _ultFirstRun;

	private:
		void BulletTimeInUltimate();

		void RunUltimateAnimation();

		void EndUltimateAnimation();

		void StartSwordEffect();

		void EndSwordEffect();

		void StartSwordTrail();

		void EndSwordTrail();

		void ChargingFist();

		void EndCharging();

		void FistWaveHit();

		void SwordWaveHit();

		DUOLGameEngine::CoroutineHandler StartBulletTIme(float duration);

		DUOLGameEngine::CoroutineHandler LaunchWave();

		void StartSwordAttackFrame();

		void EndSwordAttackFrame();

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}
