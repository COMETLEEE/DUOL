#include "DUOLGameEngine/ECS/Object/Material.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"

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