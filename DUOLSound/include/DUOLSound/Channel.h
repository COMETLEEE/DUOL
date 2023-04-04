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

#include "DUOLMath/DUOLMath.h"

namespace DUOLSound
{
	/**
	 * \brief represents each sound's play context.
	 */
	class Channel
	{
	public:
		Channel(FMOD::Channel* channel);

		~Channel();

	private:
		FMOD::Channel* _channel;

	public:
		void Set3DAttribute(const DUOLMath::Vector3& worldPos, const DUOLMath::Vector3& velocity);
	};
}