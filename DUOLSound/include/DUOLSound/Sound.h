/**

    @file      Sound.h
    @brief     사운드 파일, 런타임 재생에 대한 정보를 가지고 있는 클래스.
    @details   ~
    @author    COMETLEE
    @date      4.04.2023
    @copyright © COMETLEE, 2023. All right reserved.

**/
#pragma once
#include "FMOD/fmod.hpp"

#include "DUOLSound/Util/Export.h"
#include "DUOLSound/Channel.h"

namespace DUOLSound
{
	class Channel;

	class System;
}

namespace DUOLSound
{
	/**
	 * \brief 사운드 파일, 재생에 대한 정보를 담고 있는 클래스. 시스템으로부터 생성된다.
	 */
	class DUOL_SOUND_API Sound
	{
	public:
		Sound(FMOD::Sound* sound, FMOD::System* system, const DUOLSound::SoundFlags& soundFlags);

		~Sound();

	private:
		FMOD::Sound*			_sound;

		FMOD::System*			_system;

		/**
		 * \brief Default common setting of sound.
		 */
		SoundFlags				_flags;

	private:
		void Release();

	public:
		/**
		 * \brief 
		 * \param paused 채널의 생성 시점 상태. 해당 사운드 담당.
		 * \return 배정받은 채널 개체
		 */
		DUOLSound::Channel CreateChannel(bool paused = false);

#pragma region SET_DEFAULT_SOUND_OPTIONS
		void Set2DSound();

		void Set3DSound();

		void SetLoopOn();

		void SetLoopOff();
#pragma endregion
	};
}