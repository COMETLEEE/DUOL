#include "DUOLGameEngine/ECS/Object/AudioClip.h"


#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
#include "DUOLSound/Sound.h"
using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::AudioClip>("AudioClip")
	.constructor<>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

namespace DUOLGameEngine
{
	AudioClip::AudioClip(const DUOLCommon::tstring& name) :
		DUOLGameEngine::ObjectBase(name, ObjectType::Resource)
	{
	}

	AudioClip::~AudioClip()
	{
	}

	void AudioClip::SetLoopOff()
	{
		_sound->SetLoopOff();
	}

	void AudioClip::SetLoopOn()
	{
		_sound->SetLoopOn();
	}

	void AudioClip::Set2DSound()
	{
		_sound->Set2DSound();
	}

	void AudioClip::Set3DSound()
	{
		_sound->Set3DSound();
	}
}