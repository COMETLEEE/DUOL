#include "DUOLGameEngine/ECS/Object/AudioClip.h"


#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
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
}