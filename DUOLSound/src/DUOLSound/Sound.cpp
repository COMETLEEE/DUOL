#include "DUOLSound/Sound.h"

#include "DUOLSound/Util/ErrorCheck.h"

namespace DUOLSound
{
	Sound::Sound(FMOD::Sound* sound, FMOD::System* system) :
		_sound(sound)
		, _system(system)
	{
	}

	Sound::~Sound()
	{
		Release();
	}

	void Sound::Release()
	{
		if (_sound != nullptr)
		{
			FMOD_RESULT result{};

			result = _sound->release();

			ERROR_CHECK(result)
		}
	}

	DUOLSound::Channel Sound::PlaySound(bool paused)
	{
		FMOD_RESULT result{};

		FMOD::Channel* channel;

		result = _system->playSound(_sound, nullptr, paused, &channel);

		ERROR_CHECK(result)

		return DUOLSound::Channel(channel);
	}
}