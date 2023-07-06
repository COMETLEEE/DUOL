/**
	@file      EnemySoundManager.h
	@brief     EnemySoundManager, 몬스터의 사운드를 관리하는 사운드 큐. 사운드 매니저라고 생각하자.
	@details   ~ 몬스터 갯수만큼 소리가 다 출력되면 너무 소리가 크고 시끄러울 것, 일정 갯수 이상은 출력하지 말자...!
	@author    SinSeongHyeon
	@date      3.04.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.
**/
#pragma once
#include <unordered_map>

#include "DUOLCommon/StringHelper.h"

namespace DUOLGameEngine
{
	class AudioClip;
}
namespace DUOLClient
{
	// const DUOLCommon::tstring SoundPath[]
	// 기존에 작성 되어 있던 Enum 순서 바꾸지 말 것.
	// 문제가 생길수도 있음...!!
	// 가능한 순서는 바꾸지 말고 아래쪽에 추가할 것.
	enum class EnemyAudioEnum
	{
		None,

		CloseAttack,
		Monster01,
		Monster02,
		Monster03,
		Monster04,
		Monster05,
		Monster06,
		Monster07,
		Monster08,
		Monster09,
		Simple_Shot,

		EliteCloseAttack,
		EliteMonster_Die,
		EliteMonster_idle01,
		EliteMonster_idle02,
		EliteMonster_Lock,
		EliteMonsterHit,
		EliteMonsterJump,
		EliteMonsterNormalAttack,
		MidBossCrack,
		MiddleBossPunch,
		MiddleBossPunchCharge,
		MiddleBossRoar,
		MiddleBossFootStep01,
		MiddleBossFootStep02,
		Elite_HeavyWhoosh,
		Elite_HeavyBigWhoosh,

		Eruption,
		FlyingSlash,
		FootStep01,
		FootStep02,
		avoidSound,
		Hit_Sound_Effect,
		Hitting_Ground,
		Slash_One,
		Slash_Two,
		Slash_Final,
		OverdriveSword01,
		OverdriveSword02,
		OverdriveSwordFinal,
		Overdrive_Fist_One,
		Overdrive_Fist_Two,
		MagnumPunch,
		SwordChargingSound01,
		SwordChargingSound02,
		FistChargingSound,
		UltimateMagnumPunch,
		Normal_Last_Punch,
		Fist_Two,

		Count
	};

	class EnemyAudioManager
	{
	public:
		EnemyAudioManager() = default;

		~EnemyAudioManager() = default;

	private:
		/**
		* \brief [Sound Enum, Sound Count] 페어로 관리한다.
		*/
		std::unordered_map<EnemyAudioEnum, int>	_audioClipsCount;

		/**
		* \brief [Sound Enum, AudioClip] 페어로 관리한다.
		*/
		std::unordered_map<EnemyAudioEnum, DUOLGameEngine::AudioClip*>  _audioClips;

		void NormalEnemyAudioClipTable();

		void EliteEnemyAudioClipTable();

		void BossEnemyAudioClipTable();

		const int _maxSoundCount = 4;
	public:
		void Initialize();

		DUOLGameEngine::AudioClip* GetAudioClip(EnemyAudioEnum enemySoundEnum);

		void ReturnAudioClip(EnemyAudioEnum enemySoundEnum);
	};
}
