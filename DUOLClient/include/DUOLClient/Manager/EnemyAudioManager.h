﻿/**
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
	//const DUOLCommon::tstring SoundPath[]

	enum class EnemyAudioEnum
	{
		NormalEnemyHit,
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

	public:
		void Initialize();

		DUOLGameEngine::AudioClip* GetAudioClip(EnemyAudioEnum enemySoundEnum);

		void ReturnAudioClip(EnemyAudioEnum enemySoundEnum);
	};
}