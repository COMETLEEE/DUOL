#pragma once
#include "DUOLClient/Player/FSM/PlayerStateBase.h"

namespace DUOLGameEngine
{
	class AudioClip;
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

		DUOLGameEngine::ParticleRenderer* _rightFistFormAura;

		bool _ultFirstRun;

		std::unordered_map<int, DUOLGameEngine::AudioClip*>  _audioClips;

	private:
		void BulletTimeInUltimate();

		void EndBulletTimeInUltimate();

		void RunUltimateAnimation();

		void EndUltimateAnimation();

		void StartSwordTrail();

		void EndSwordTrail();

		void ChargingFist();

		void ChargingSword();

		void EndCharging();

		void FistWaveHit();

		void SwordWaveHit();

		DUOLGameEngine::CoroutineHandler StartBulletTIme(float duration);

		DUOLGameEngine::CoroutineHandler LaunchWave();

		DUOLGameEngine::CoroutineHandler LaunchAreaWave();

		void StartSwordAttackFrame();

		void EndSwordAttackFrame();

		void PlaySoundEffect(PlayerSoundTable sounds);

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}
