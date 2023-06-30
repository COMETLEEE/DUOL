#include "DUOLClient/Manager/EnemyAudioManager.h"

#include "DUOLGameEngine/Manager/SoundManager.h"

namespace DUOLClient
{
	void EnemyAudioManager::Initialize()
	{

		for (int i = 0; i < static_cast<int>(EnemyAudioEnum::Count); i++)
		{
			_audioClipsCount[static_cast<EnemyAudioEnum>(i)] = _maxSoundCount;
			_audioClips[static_cast<EnemyAudioEnum>(i)] = nullptr;
		}
		NormalEnemyAudioClipTable();

		EliteEnemyAudioClipTable();
		// 오디오 클립을 미리 가져와서 저장한다.
	}


	void EnemyAudioManager::NormalEnemyAudioClipTable()
	{
		_audioClips[EnemyAudioEnum::CloseAttack] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/NormalMonster/CloseAttack.wav"), TEXT("CloseAttack"));

		_audioClips[EnemyAudioEnum::Monster01] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/NormalMonster/Monster01.wav"), TEXT("Monster01"));

		_audioClips[EnemyAudioEnum::Monster02] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/NormalMonster/Monster02.wav"), TEXT("Monster02"));

		_audioClips[EnemyAudioEnum::Monster03] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/NormalMonster/Monster03.wav"), TEXT("Monster03"));

		_audioClips[EnemyAudioEnum::Monster04] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/NormalMonster/Monster04.wav"), TEXT("Monster04"));

		_audioClips[EnemyAudioEnum::Monster05] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/NormalMonster/Monster05.wav"), TEXT("Monster05"));

		_audioClips[EnemyAudioEnum::Monster06] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/NormalMonster/Monster06.wav"), TEXT("Monster06"));

		_audioClips[EnemyAudioEnum::Monster07] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/NormalMonster/Monster07.wav"), TEXT("Monster07"));

		_audioClips[EnemyAudioEnum::Monster08] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/NormalMonster/Monster08.wav"), TEXT("Monster08"));

		_audioClips[EnemyAudioEnum::Monster09] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/NormalMonster/Monster09.wav"), TEXT("Monster09"));

		_audioClips[EnemyAudioEnum::Simple_Shot] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/NormalMonster/Simple_Shot.wav"), TEXT("Simple_Shot"));
	}

	void EnemyAudioManager::EliteEnemyAudioClipTable()
	{
		_audioClips[EnemyAudioEnum::EliteCloseAttack] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/EliteMonster/EliteCloseAttack.wav"), TEXT("EliteCloseAttack"));

		_audioClips[EnemyAudioEnum::EliteMonster_Die] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/EliteMonster/EliteMonster_Die.wav"), TEXT("EliteMonster_Die"));

		_audioClips[EnemyAudioEnum::EliteMonster_idle01] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/EliteMonster/EliteMonster_idle01.wav"), TEXT("EliteMonster_idle01"));

		_audioClips[EnemyAudioEnum::EliteMonster_idle02] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/EliteMonster/EliteMonster_idle02.wav"), TEXT("EliteMonster_idle02"));

		_audioClips[EnemyAudioEnum::EliteMonster_Lock] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/EliteMonster/EliteMonster_Lock.wav"), TEXT("EliteMonster_Lock"));

		_audioClips[EnemyAudioEnum::EliteMonsterHit] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/EliteMonster/EliteMonsterHit.wav"), TEXT("EliteMonsterHit"));

		_audioClips[EnemyAudioEnum::EliteMonsterJump] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/EliteMonster/EliteMonsterJump.wav"), TEXT("EliteMonsterJump"));

		_audioClips[EnemyAudioEnum::EliteMonsterNormalAttack] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/EliteMonster/EliteMonsterNormalAttack.wav"), TEXT("EliteMonsterNormalAttack"));

		_audioClips[EnemyAudioEnum::MidBossCrack] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/EliteMonster/MidBossCrack.wav"), TEXT("MidBossCrack"));

		_audioClips[EnemyAudioEnum::MiddleBossPunch] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/EliteMonster/MiddleBossPunch.wav"), TEXT("MiddleBossPunch"));

		_audioClips[EnemyAudioEnum::MiddleBossPunchCharge] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/EliteMonster/MiddleBossPunchCharge.wav"), TEXT("MiddleBossPunchCharge"));

		_audioClips[EnemyAudioEnum::MiddleBossRoar] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/EliteMonster/MiddleBossRoar.wav"), TEXT("MiddleBossRoar"));

		_audioClips[EnemyAudioEnum::MiddleBossFootStep01] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/EliteMonster/MiddleBossFootStep01.wav"), TEXT("MiddleBossFootStep01"));

		_audioClips[EnemyAudioEnum::MiddleBossFootStep02] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/EliteMonster/MiddleBossFootStep02.wav"), TEXT("MiddleBossFootStep02"));

		_audioClips[EnemyAudioEnum::Elite_HeavyBigWhoosh] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/EliteMonster/Elite_HeavyBigWhoosh.wav"), TEXT("Elite_HeavyBigWhoosh"));

		_audioClips[EnemyAudioEnum::Elite_HeavyWhoosh] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/EliteMonster/Elite_HeavyWhoosh.wav"), TEXT("Elite_HeavyWhoosh"));
	}


	DUOLGameEngine::AudioClip* EnemyAudioManager::GetAudioClip(EnemyAudioEnum enemySoundEnum)
	{
		if (_audioClipsCount[enemySoundEnum] > 0)
		{
			_audioClipsCount[enemySoundEnum]--;
			return _audioClips[enemySoundEnum];
		}
		return nullptr;
	}

	void EnemyAudioManager::ReturnAudioClip(EnemyAudioEnum enemySoundEnum)
	{
		_audioClipsCount[enemySoundEnum]++;
	}
}
