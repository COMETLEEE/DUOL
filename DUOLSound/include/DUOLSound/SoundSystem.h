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
#include "Util/ConverVector.h"

namespace DUOLSound
{
	class Sound;
}

namespace DUOLSound
{
	/**
	 * \brief 3D Listener 의 특성
	 */
	struct DUOL_SOUND_API ListenerAttribute
	{
		int _index = 0;

		DUOLMath::Vector3 _pos;

		DUOLMath::Vector3 _velocity;

		DUOLMath::Vector3 _forward;

		DUOLMath::Vector3 _up;
	};

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

		DUOLSound::Sound* GetSound(const DUOLCommon::tstring& soundFilePath);

		void Get3DListenerCount(int& outCount);

		void Set3DListenerCount(int count);

		void Get3DListenerAttribute(ListenerAttribute& outAttribute);

		void Set3DListenerAttribute(const ListenerAttribute& attribute);

		void Update();
	};
}