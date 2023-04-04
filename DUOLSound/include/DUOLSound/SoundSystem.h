/**

    @file      SoundSystem.h
    @brief     사운드 출력 업무를 외주받는 클래스.
    @details   ~
    @author    COMETLEE
    @date      4.04.2023
    @copyright © COMETLEE, 2023. All right reserved.

**/
#pragma once
#include <memory>
#include <unordered_map>

#include "FMOD/fmod.hpp"

#include "DUOLSound/Util/Export.h"
#include "DUOLCommon/StringHelper.h"
#include "DUOLSound/Util/Descriptions.h"

namespace DUOLSound
{
	class Sound;
}

namespace DUOLSound
{
	/**
	 * \brief 사운드 미들웨어를 사용해 사운드 관련된 일을 수행하는 클래스
	 */
	class DUOL_SOUND_API SoundSystem
	{
	public:
		SoundSystem();

		~SoundSystem();

	private:
		/**
		 * \brief FMOD System
		 */
		std::unique_ptr<FMOD::System> _system;

		/**
		 * \brief FMOD Sounds
		 */
		std::unordered_map<DUOLCommon::tstring, std::unique_ptr<DUOLSound::Sound>> _sounds;

	public:
		void Initialize(const SOUNDSYSTEM_SYSTEM_DESC& desc);

		void UnInitialize();

		DUOLSound::Sound* CreateSound(const SOUNDSYSTEM_SOUND_DESC& desc);

		bool DeleteSound(const DUOLCommon::tstring& soundFilePath);

		void Update();
	};
}