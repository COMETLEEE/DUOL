#include "DUOLSound/Channel.h"

#include "DUOLSound/Util/ConverVector.h"
#include "DUOLSound/Util/ErrorCheck.h"

namespace DUOLSound
{
	Channel::Channel(FMOD::Channel* channel) :
		_channel(channel)
	{

	}

	Channel::~Channel()
	{

	}

	void Channel::Set3DAttribute(const DUOLMath::Vector3& worldPos, const DUOLMath::Vector3& velocity)
	{
		FMOD_RESULT result{};

		FMOD_VECTOR worldPosVec = ConverVector3(worldPos);
		FMOD_VECTOR velocityVec = ConverVector3(velocity);

		result = _channel->set3DAttributes(&worldPosVec, &velocityVec);

		ERROR_CHECK(result)
	}
}