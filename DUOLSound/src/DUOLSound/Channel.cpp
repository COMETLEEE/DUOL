#include "DUOLSound/Channel.h"

#include "DUOLSound/Util/ConverVector.h"
#include "DUOLSound/Util/ErrorCheck.h"

namespace DUOLSound
{
	Channel::Channel(FMOD::Channel* channel) :
		_channel(channel)
	{
		if (_channel != nullptr)
			_channel->getMode(reinterpret_cast<FMOD_MODE*>(&_flags));
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

	void Channel::GetMaxDistance(float& outMaxDistance) const
	{
		float minDist;

		_channel->get3DMinMaxDistance(&minDist, &outMaxDistance);
	}

	void Channel::SetMaxDistance(float maxDistance)
	{
		float minDist;

		float maxDist;

		_channel->get3DMinMaxDistance(&minDist, &maxDist);

		_channel->set3DMinMaxDistance(minDist, maxDistance);
	}

	void Channel::GetMinDistance(float& outMinDistance) const
	{
		float maxDist;

		_channel->get3DMinMaxDistance(&outMinDistance, &maxDist);
	}

	void Channel::SetMinDistance(float minDistance)
	{
		float minDist;

		float maxDist;

		_channel->get3DMinMaxDistance(&minDist, &maxDist);

		_channel->set3DMinMaxDistance(minDistance, maxDist);
	}

	void Channel::GetVolume(float& outVolume) const
	{
		_channel->getVolume(&outVolume);
	}

	void Channel::SetVolume(float volume)
	{
		_channel->setVolume(volume);
	}

	void Channel::GetMute(bool& outMute) const
	{
		_channel->getMute(&outMute);
	}

	void Channel::SetMute(bool value)
	{
		_channel->setMute(value);
	}

	void Channel::GetIsLoop(bool& outLoop)
	{
		auto fl = _flags & SOUNDSYSTEM_SOUND_FLAG::SOUND_LOOP_NORMAL;

		outLoop = (fl == SOUNDSYSTEM_SOUND_FLAG::SOUND_LOOP_NORMAL);
	}

	void Channel::SetIsLoop(bool value)
	{
		if (value)
		{
			_flags |= SOUNDSYSTEM_SOUND_FLAG::SOUND_LOOP_NORMAL;
		}
		else
		{
			_flags &= ~SoundFlags(SOUNDSYSTEM_SOUND_FLAG::SOUND_LOOP_NORMAL);

			_flags |= SOUNDSYSTEM_SOUND_FLAG::SOUND_LOOP_OFF;
		}

		_channel->setMode(static_cast<FMOD_MODE>(_flags.GetBitMask()));
	}

	void Channel::IsPlaying(bool& outIsPlaying) const
	{
		_channel->isPlaying(&outIsPlaying);
	}

	void Channel::GetPriority(int& outPriority)
	{
		_channel->getPriority(&outPriority);
	}

	void Channel::SetPriority(int priority)
	{
		_channel->setPriority(priority);
	}

	void Channel::GetDopplerLevel(float& outDopplerLevel)
	{
		_channel->get3DDopplerLevel(&outDopplerLevel);
	}

	void Channel::SetDopplerLevel(float dopplerLevel)
	{
		_channel->set3DDopplerLevel(dopplerLevel);
	}

	void Channel::SetPosition(unsigned position)
	{
		_channel->setPosition(position, FMOD_TIMEUNIT_MS);
	}


	void Channel::Stop()
	{
		_channel->stop();

		// 채널 개체 자동 반환
	}

	void Channel::UnPause()
	{
		_channel->setPaused(false);
	}

	void Channel::Pause()
	{
		_channel->setPaused(true);
	}
}