#include "DUOLSound/SoundSystem.h"

#include "DUOLSound/Sound.h"
#include "DUOLSound/Util/ErrorCheck.h"

namespace DUOLSound
{
	using namespace FMOD;

	SoundSystem::SoundSystem() :
		_system(nullptr)
		, _sounds {}
	{
	}

	SoundSystem::~SoundSystem()
	{
		UnInitialize();
	}

	void SoundSystem::Initialize(const SOUNDSYSTEM_SYSTEM_DESC& desc)
	{
		FMOD_RESULT result {};

		FMOD::System* system;

		result = FMOD::System_Create(&system);

		ERROR_CHECK(result)

		_system = std::unique_ptr<FMOD::System>(system);

		auto bitMask = desc._flags.GetBitMask();

		_system->init(desc._maxChannel, bitMask, desc._extraDriverData);
	}

	void SoundSystem::UnInitialize()
	{
		for (auto& [name, sound] : _sounds)
		{
			sound.reset();
		}

		// release system.
		_system->release();
	}

	DUOLSound::Sound* SoundSystem::CreateSound(const SOUNDSYSTEM_SOUND_DESC& desc)
	{
		FMOD_RESULT result{};

		FMOD::Sound* sound;

		auto bitMask = desc._flags.GetBitMask();

		result = _system->createSound(desc._soundFilePath, bitMask, nullptr, &sound);

		ERROR_CHECK(result)

		_sounds.insert({ DUOLCommon::StringHelper::ToTString(desc._soundFilePath), std::make_unique<DUOLSound::Sound>(sound, _system.get()) });

		return _sounds.at(DUOLCommon::StringHelper::ToTString(desc._soundFilePath)).get();
	}

	bool SoundSystem::DeleteSound(const DUOLCommon::tstring& soundFilePath)
	{
		if (_sounds.contains(soundFilePath))
		{
			_sounds.erase(soundFilePath);

			return true;
		}

		return false;
	}
}