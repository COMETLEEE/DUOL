#include "DUOLGameEngine/ECS/Object/Material.h"

namespace DUOLGameEngine
{
	Material::Material(const DUOLCommon::tstring& name) :
		DUOLGameEngine::ObjectBase(name, ObjectType::Resource)
	{

	}

	Material::~Material()
	{

	}
}