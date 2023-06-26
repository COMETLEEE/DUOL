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
	 * \brief represents each sound play's context.
	 */
	class DUOL_SOUND_API Channel
	{
	public:
		Channel(FMOD::Channel* channel);

		~Channel();

	private:
		/**
		 * \brief 채널입니다. Stop 시 자동 반환됩니다.
		 */
		FMOD::Channel* _channel;

		/**
		 * \brief
		 */
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

		void IsPlaying(bool& outIsPlaying) const;

		void GetPriority(int& outPriority);

		void SetPriority(int priority);

		void GetDopplerLevel(float& outDopplerLevel);

		void SetDopplerLevel(float dopplerLevel);

		void SetPosition(unsigned position);

		void Stop();

		void UnPause();

		void Pause();
	};
}