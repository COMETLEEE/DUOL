/**

    @file      SoundPlayContext.h
    @brief     Represents current sound playing context.
    @details   ~
    @author    COMETLEE
    @date      4.04.2023
    @copyright © COMETLEE, 2023. All right reserved.

**/
#pragma once
#include "DUOLSound/Channel.h"

namespace DUOLSound
{
	/**
	 * \brief represents context of sound playing.
	 */
	class SoundPlayContext
	{
	public:
		SoundPlayContext(DUOLSound::Channel channel) : _channel(channel) {}

		~SoundPlayContext() = default;

		SoundPlayContext(SoundPlayContext&) = default;

		SoundPlayContext(SoundPlayContext&&) = default;

	public:
		DUOLSound::Channel _channel;

	public:
		void Set3DAttribute(const DUOLMath::Vector3& worldPos, const DUOLMath::Vector3& velocity = DUOLMath::Vector3::Zero);
	};
}
