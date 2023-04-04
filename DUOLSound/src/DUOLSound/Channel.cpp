#include "DUOLSound/Channel.h"

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
	}
}