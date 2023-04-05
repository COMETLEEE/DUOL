#include "DUOLSound/Sound.h"

#include "DUOLSound/Util/ErrorCheck.h"

namespace DUOLSound
{
	Sound::Sound(FMOD::Sound* sound, FMOD::System* system, const DUOLSound::SoundFlags& soundFlags) :
		_sound(sound)
		, _system(system)
		, _flags(soundFlags)
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

	void Sound::Set2DSound()
	{
		_flags &= ~SoundFlags(SOUNDSYSTEM_SOUND_FLAG::SOUND_3D);

		_flags |= SOUNDSYSTEM_SOUND_FLAG::SOUND_2D;

		_sound->setMode(_flags.GetBitMask());
	}

	void Sound::Set3DSound()
	{
		_flags &= ~SoundFlags(SOUNDSYSTEM_SOUND_FLAG::SOUND_2D);

		_flags |= SOUNDSYSTEM_SOUND_FLAG::SOUND_3D;

		_sound->setMode(_flags.GetBitMask());
	}

	void Sound::SetLoopOn()
	{
		_flags &= ~SoundFlags(SOUNDSYSTEM_SOUND_FLAG::SOUND_LOOP_OFF);

		_flags |= SOUNDSYSTEM_SOUND_FLAG::SOUND_LOOP_NORMAL;

		_sound->setMode(_flags.GetBitMask());
	}

	void Sound::SetLoopOff()
	{
		_flags &= ~SoundFlags(SOUNDSYSTEM_SOUND_FLAG::SOUND_LOOP_NORMAL);

		_flags |= SOUNDSYSTEM_SOUND_FLAG::SOUND_LOOP_OFF;

		_sound->setMode(_flags.GetBitMask());
	}

	DUOLSound::Channel Sound::CreateChannel(bool paused)
	{
		FMOD_RESULT result{};

		FMOD::Channel* channel;

		result = _system->playSound(_sound, nullptr, paused, &channel);

		ERROR_CHECK(result)

		return DUOLSound::Channel(channel);
	}
}