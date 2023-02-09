#include "DUOLGameEngine/ECS/Object/Material.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::Material>("Material")
	.constructor<>()
	(

	);
}

namespace DUOLGameEngine
{
	Material::Material(const DUOLCommon::tstring& name) :
		DUOLGameEngine::ObjectBase(name, ObjectType::Resource)
	{

	}

	Material::~Material()
	{

	}

	void Material::SetAlbedo(const DUOLMath::Vector4& albedo)
	{
		_primitiveMaterial->SetAlbedo(albedo);
	}
}