#include "DUOLGameEngine/ECS/Object/Sprite.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::Sprite>("Sprite")
	.constructor<>()
	(

	);
}

namespace DUOLGameEngine
{
	Sprite::Sprite(const DUOLCommon::tstring& name) :
		DUOLGameEngine::ObjectBase(name, ObjectType::Resource)
		, _sprite(new DUOLGraphicsLibrary::Sprite())
	{

	}

	Sprite::~Sprite()
	{
		delete _sprite;
	}

	void Sprite::SetSpriteName(const DUOLCommon::tstring& name)
	{
		this->SetName(name);
	}
}