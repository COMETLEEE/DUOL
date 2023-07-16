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

		BossEnemyAudioClipTable();
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

	void EnemyAudioManager::BossEnemyAudioClipTable()
	{
		_audioClips[EnemyAudioEnum::Eruption] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/Boss/Eruption.wav"), TEXT("Eruption"));

		_audioClips[EnemyAudioEnum::FlyingSlash] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/Boss/FlyingSlash.wav"), TEXT("FlyingSlash"));

		_audioClips[EnemyAudioEnum::FootStep01] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/PlayerSFX/FootStep01.wav"), TEXT("FootStep01"));

		_audioClips[EnemyAudioEnum::FootStep02] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/PlayerSFX/FootStep02.wav"), TEXT("FootStep02"));

		_audioClips[EnemyAudioEnum::avoidSound] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/PlayerSFX/avoidSound.wav"), TEXT("avoidSound"));

		_audioClips[EnemyAudioEnum::Hit_Sound_Effect] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/PlayerSFX/Hit_Sound_Effect.wav"), TEXT("Hit_Sound_Effect"));

		_audioClips[EnemyAudioEnum::Hitting_Ground] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/PlayerSFX/Hitting_Ground.wav"), TEXT("Hitting_Ground"));

		_audioClips[EnemyAudioEnum::Slash_One] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/PlayerSFX/Slash_One.wav"), TEXT("Slash_One"));

		_audioClips[EnemyAudioEnum::Slash_Two] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/PlayerSFX/Slash_Two.wav"), TEXT("Slash_Two"));

		_audioClips[EnemyAudioEnum::Slash_Final] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/PlayerSFX/Slash_Final.wav"), TEXT("Slash_Final"));

		_audioClips[EnemyAudioEnum::OverdriveSword01] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/PlayerSFX/OverdriveSword1.wav"), TEXT("OverdriveSword01"));

		_audioClips[EnemyAudioEnum::OverdriveSword02] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/PlayerSFX/OverdriveSword2.wav"), TEXT("OverdriveSword02"));

		_audioClips[EnemyAudioEnum::OverdriveSwordFinal] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/PlayerSFX/OverdriveSwordFinal.wav"), TEXT("OverdriveSwordFinal"));

		_audioClips[EnemyAudioEnum::Overdrive_Fist_One] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/PlayerSFX/Overdrive_Fist_One.wav"), TEXT("Overdrive_Fist_One"));

		_audioClips[EnemyAudioEnum::Overdrive_Fist_Two] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/PlayerSFX/Overdrive_Fist_Two.wav"), TEXT("Overdrive_Fist_Two"));

		_audioClips[EnemyAudioEnum::MagnumPunch] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/PlayerSFX/MagnumPunch.wav"), TEXT("MagnumPunch"));

		_audioClips[EnemyAudioEnum::SwordChargingSound01] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/PlayerSFX/SwordChargingSound01.wav"), TEXT("SwordChargingSound01"));

		_audioClips[EnemyAudioEnum::SwordChargingSound02] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/PlayerSFX/SwordChargingSound02.wav"), TEXT("SwordChargingSound02"));

		_audioClips[EnemyAudioEnum::FistChargingSound] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/PlayerSFX/FistChargingSound.wav"), TEXT("FistChargingSound"));

		_audioClips[EnemyAudioEnum::UltimateMagnumPunch] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/PlayerSFX/UltimateMagnumPunch.wav"), TEXT("UltimateMagnumPunch"));

		_audioClips[EnemyAudioEnum::Normal_Last_Punch] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/PlayerSFX/Normal_Last_Punch.wav"), TEXT("Normal_Last_Punch"));

		_audioClips[EnemyAudioEnum::Fist_Two] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/PlayerSFX/Fist_Two.wav"), TEXT("Fist_Two"));

		// ------------------------------------------- Voice Sound -------------------------------------

		_audioClips[EnemyAudioEnum::Voice_Boss_OverdriveSword] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/BossActionVoice/001Voice_Boss_OverdriveSword.wav"), TEXT("Voice_Boss_OverdriveSword"));

		_audioClips[EnemyAudioEnum::Voice_Boss_FlySlash_A] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/BossActionVoice/002Voice_Boss_FlySlash_A.wav"), TEXT("Voice_Boss_FlySlash_A"));

		_audioClips[EnemyAudioEnum::Voice_Boss_FlySlash_B] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/BossActionVoice/002Voice_Boss_FlySlash_B.wav"), TEXT("Voice_Boss_FlySlash_B"));

		_audioClips[EnemyAudioEnum::Voice_Boss_Dash_A] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/BossActionVoice/003Voice_Boss_Dash_A.wav"), TEXT("Voice_Boss_Dash_A"));

		_audioClips[EnemyAudioEnum::Voice_Boss_Dash_B] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/BossActionVoice/003Voice_Boss_Dash_B.wav"), TEXT("Voice_Boss_Dash_B"));

		_audioClips[EnemyAudioEnum::Voice_Boss_Pyeongta_A] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/BossActionVoice/004Voice_Boss_Pyeongta_A.wav"), TEXT("Voice_Boss_Pyeongta_A"));

		_audioClips[EnemyAudioEnum::Voice_Boss_Pyeongta_B] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/BossActionVoice/004Voice_Boss_Pyeongta_B.wav"), TEXT("Voice_Boss_Pyeongta_B"));

		_audioClips[EnemyAudioEnum::Voice_Boss_Combo_A] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/BossActionVoice/005Voice_Boss_Combo_A.wav"), TEXT("Voice_Boss_Combo_A"));

		_audioClips[EnemyAudioEnum::Voice_Boss_Combo_B] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/BossActionVoice/005Voice_Boss_Combo_B.wav"), TEXT("Voice_Boss_Combo_B"));

		_audioClips[EnemyAudioEnum::Voice_Boss_UltimateSword_A] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/BossActionVoice/006Voice_Boss_UltimateSword_A.wav"), TEXT("Voice_Boss_UltimateSword_A"));

		_audioClips[EnemyAudioEnum::Voice_Boss_UltimateSword_B] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/BossActionVoice/006Voice_Boss_UltimateSword_B.wav"), TEXT("Voice_Boss_UltimateSword_B"));

		_audioClips[EnemyAudioEnum::Voice_Boss_UltimateSword_C] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/BossActionVoice/006Voice_Boss_UltimateSword_C.wav"), TEXT("Voice_Boss_UltimateSword_C"));

		_audioClips[EnemyAudioEnum::Voice_Boss_OverdriveFist] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/BossActionVoice/007Voice_Boss_OverdriveFist.wav"), TEXT("Voice_Boss_OverdriveFist"));

		_audioClips[EnemyAudioEnum::Voice_Boss_Jump] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/BossActionVoice/008Voice_Boss_Jump.wav"), TEXT("Voice_Boss_Jump"));

		_audioClips[EnemyAudioEnum::Voice_Boss_MagnumPunch] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/BossActionVoice/008Voice_Boss_MagnumPunch.wav"), TEXT("Voice_Boss_MagnumPunch"));

		_audioClips[EnemyAudioEnum::Voice_Boss_UltimateFist_A] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/BossActionVoice/009Voice_Boss_UltimateFist_A.wav"), TEXT("Voice_Boss_UltimateFist_A"));

		_audioClips[EnemyAudioEnum::Voice_Boss_UltimateFist_B] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/BossActionVoice/009Voice_Boss_UltimateFist_B.wav"), TEXT("Voice_Boss_UltimateFist_B"));

		_audioClips[EnemyAudioEnum::Voice_Boss_Hit_A] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/BossActionVoice/010Voice_Boss_Hit_A.wav"), TEXT("Voice_Boss_Hit_A"));

		_audioClips[EnemyAudioEnum::Voice_Boss_Hit_B] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/BossActionVoice/010Voice_Boss_Hit_B.wav"), TEXT("Voice_Boss_Hit_B"));

		_audioClips[EnemyAudioEnum::Voice_Boss_Die_A] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/BossActionVoice/011Voice_Boss_Die_A.wav"), TEXT("Voice_Boss_Die_A"));

		_audioClips[EnemyAudioEnum::Voice_Boss_Die_B] = DUOLGameEngine::SoundManager::GetInstance()->CreateAudioClip(TEXT("Asset/Sound/BossActionVoice/011Voice_Boss_Die_B.wav"), TEXT("Voice_Boss_Die_B"));
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
