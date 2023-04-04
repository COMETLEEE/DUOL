#pragma once
#include <cstdint>

#include "DUOLSound/Util/Flags.h"

namespace DUOLSound
{
#pragma region SYSTEM
	enum class SOUNDSYSTEM_SYSTEMINIT_FLAGS : uint32_t
	{
		INIT_NORMAL							= 0x00000000
		, INIT_STREAM_FROM_UPDATE			= 0x00000001
		, INIT_MIX_FROM_UPDATE				= 0x00000002
		, INIT_3D_RIGHTHANDED				= 0x00000004
		, INIT_CLIP_OUTPUT					= 0x00000008
		, INIT_CHANNEL_LOWPASS				= 0x00000100
		, INIT_CHANNEL_DISTANCEFILTER		= 0x00000200
		, INIT_PROFILE_ENABLE				= 0x00010000
		, INIT_VOL0_BECOMES_VIRTUAL			= 0x00020000
		, INIT_GEOMETRY_USECLOSEST			= 0x00040000
		, INIT_PREFER_DOLBY_DOWNMIX			= 0x00080000
		, INIT_THREAD_UNSAFE				= 0x00100000
		, INIT_PROFILE_METER_ALL			= 0x00200000
		, INIT_MEMORY_TRACKING				= 0x00400000
	};

	using SystemFlags = DUOLSound::Flags<SOUNDSYSTEM_SYSTEMINIT_FLAGS, uint32_t>;

	struct SOUNDSYSTEM_SYSTEM_DESC
	{
		int _maxChannel = 32;

		SystemFlags _flags = SOUNDSYSTEM_SYSTEMINIT_FLAGS::INIT_NORMAL;

		void* _extraDriverData = nullptr;
	};
#pragma endregion

#pragma region SOUND
	enum class SOUNDSYSTEM_SOUND_FLAG : uint32_t
	{
		SOUND_DEFAULT								 =  0x00000000
		, SOUND_LOOP_OFF                             =  0x00000001
		, SOUND_LOOP_NORMAL                          =  0x00000002
		, SOUND_LOOP_BIDI                            =  0x00000004
		, SOUND_2D                                   =  0x00000008
		, SOUND_3D                                   =  0x00000010
		, SOUND_CREATESTREAM                         =  0x00000080
		, SOUND_CREATESAMPLE                         =  0x00000100
		, SOUND_CREATECOMPRESSEDSAMPLE               =  0x00000200
		, SOUND_OPENUSER                             =  0x00000400
		, SOUND_OPENMEMORY                           =  0x00000800
		, SOUND_OPENMEMORY_POINT                     =  0x10000000
		, SOUND_OPENRAW                              =  0x00001000
		, SOUND_OPENONLY                             =  0x00002000
		, SOUND_ACCURATETIME                         =  0x00004000
		, SOUND_MPEGSEARCH                           =  0x00008000
		, SOUND_NONBLOCKING                          =  0x00010000
		, SOUND_UNIQUE                               =  0x00020000
		, SOUND_3D_HEADRELATIVE                      =  0x00040000
		, SOUND_3D_WORLDRELATIVE                     =  0x00080000
		, SOUND_3D_INVERSEROLLOFF                    =  0x00100000
		, SOUND_3D_LINEARROLLOFF                     =  0x00200000
		, SOUND_3D_LINEARSQUAREROLLOFF               =  0x00400000
		, SOUND_3D_INVERSETAPEREDROLLOFF             =  0x00800000
		, SOUND_3D_CUSTOMROLLOFF                     =  0x04000000
		, SOUND_3D_IGNOREGEOMETRY                    =  0x40000000
		, SOUND_IGNORETAGS                           =  0x02000000
		, SOUND_LOWMEM                               =  0x08000000
		, SOUND_VIRTUAL_PLAYFROMSTART                =  0x80000000
	};

	using SoundFlags = DUOLSound::Flags<SOUNDSYSTEM_SOUND_FLAG, uint32_t>;

	struct SOUNDSYSTEM_SOUND_DESC
	{
		const char* _soundFilePath;

		SoundFlags _flags = SOUNDSYSTEM_SOUND_FLAG::SOUND_DEFAULT;
	};
#pragma endregion
}