#include "DUOLSound/SoundSystem.h"

#include "DUOLSound/Sound.h"
#include "DUOLSound/Util/ErrorCheck.h"

#include <objbase.h>

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

		CoUninitialize();
	}

	void SoundSystem::Initialize(const SOUNDSYSTEM_SYSTEM_DESC& desc)
	{
		// FMOD 이니셜라이즈 전에 CoInitialize 필요합니다.
		CoInitializeEx(nullptr, COINIT_MULTITHREADED);

		FMOD_RESULT result {};

		FMOD::System* system;

		result = FMOD::System_Create(&system);

		ERROR_CHECK(result)

		_system = std::unique_ptr<FMOD::System>(system);

		auto bitMask = desc._flags.GetBitMask();

		_system->init(desc._maxChannel, bitMask, desc._extraDriverData);

		_system->set3DNumListeners(1);

		_system->set3DSettings(1.f, 1.f, 1.f);
	}

	void SoundSystem::UnInitialize()
	{
		for (auto& [name, sound] : _sounds)
		{
			sound.reset();
		}

		// release system.
		_system->release();

		// 제거는 위의 release에서 됐음 ..! 메모리 해제되면 안되니까 .. 빼주자.
		auto temp = _system.release();
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

	DUOLSound::Sound* SoundSystem::GetSound(const DUOLCommon::tstring& soundFilePath)
	{
		return _sounds.contains(soundFilePath) ? _sounds.at(soundFilePath).get() : nullptr;
	}

	void SoundSystem::Get3DListenerCount(int& outCount)
	{
		_system->get3DNumListeners(&outCount);
	}

	void SoundSystem::Set3DListenerCount(int count)
	{
		_system->set3DNumListeners(std::clamp<int>(count, 0, 8));
	}

	void SoundSystem::Get3DListenerAttribute(ListenerAttribute& outAttribute)
	{
		FMOD_VECTOR pos, vel, forward, up;

		_system->get3DListenerAttributes(outAttribute._index, &pos, &vel, &forward, &up);

		outAttribute._pos = ConvertFMODVector(pos);
		outAttribute._velocity = ConvertFMODVector(vel);
		outAttribute._forward = ConvertFMODVector(forward);
		outAttribute._up = ConvertFMODVector(up);
	}

	void SoundSystem::Set3DListenerAttribute(const ListenerAttribute& attribute)
	{
		FMOD_VECTOR pos = ConverVector3(attribute._pos);
		FMOD_VECTOR vel = ConverVector3(attribute._velocity);
		FMOD_VECTOR forward = ConverVector3(attribute._forward);
		FMOD_VECTOR up = ConverVector3(attribute._up);

		_system->set3DListenerAttributes(attribute._index, &pos, &vel, &forward, &up);
	}

	void SoundSystem::Update()
	{
		FMOD_RESULT result{};

		result = _system->update();

		ERROR_CHECK(result)
	}
}