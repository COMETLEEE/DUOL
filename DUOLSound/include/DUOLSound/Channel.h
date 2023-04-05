/**

    @file      Channel.h
    @brief     Handle of playing of specific sound.
    @details   ~
    @author    COMETLEE
    @date      4.04.2023
    @copyright © COMETLEE, 2023. All right reserved.

**/
#pragma once
#include <FMOD/fmod.hpp>

#include "DUOLSound/Util/Export.h"
#include "DUOLMath/DUOLMath.h"
#include "Util/Descriptions.h"

namespace DUOLSound
{
	enum class SoundRollOffType
	{
		Linear
		, LinearSquare
		, Custum
	};

	/**
	 * \brief represents each sound's play context.
	 */
	class DUOL_SOUND_API Channel
	{
	public:
		Channel(FMOD::Channel* channel);

		~Channel();

	private:
		FMOD::Sound*	_sound;

		FMOD::Channel*	_channel;

		DUOLSound::SoundFlags _flags;

	public:
		void Set3DAttribute(const DUOLMath::Vector3& worldPos, const DUOLMath::Vector3& velocity);

		void GetMaxDistance(float& outMaxDistance) const;

		void SetMaxDistance(float maxDistance);

		void GetMinDistance(float& outMinDistance) const;

		void SetMinDistance(float minDistance);

		void GetVolume(float& outVolume) const;

		void SetVolume(float volume);

		void GetMute(bool& outMute) const;

		void SetMute(bool value);

		void GetIsLoop(bool& outLoop);

		void SetIsLoop(bool value);

		void IsPlaying(bool& outIsPlaying);
	};
}
