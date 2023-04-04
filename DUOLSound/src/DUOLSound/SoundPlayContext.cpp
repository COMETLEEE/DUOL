#include "DUOLSound/SoundPlayContext.h"

namespace DUOLSound
{
	void SoundPlayContext::Set3DAttribute(const DUOLMath::Vector3& worldPos, const DUOLMath::Vector3& velocity)
	{
		_channel.Set3DAttribute(worldPos, velocity);
	}
}