#include "DUOLGameEngine/ECS/Object/Mesh.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::Mesh>("Mesh")
	.constructor<>()
	(

	);
}

namespace DUOLGameEngine
{
	Mesh::Mesh(const DUOLCommon::tstring& name) :
		DUOLGameEngine::ObjectBase(name, ObjectType::Resource)
	{

	}

	Mesh::~Mesh()
	{

	}

	bool Mesh::IsSkinnedMesh() const
	{
		return DUOLGraphicsEngine::MeshBase::MeshType::SkinnedMesh == _mesh->GetMeshType();
	}
}
