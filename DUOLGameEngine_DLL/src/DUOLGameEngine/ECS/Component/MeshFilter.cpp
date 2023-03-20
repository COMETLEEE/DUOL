#include "DUOLGameEngine/ECS/Component/MeshFilter.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::MeshFilter>("MeshFilter")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_mesh", &DUOLGameEngine::MeshFilter::_mesh)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		// 이름 검색
		, metadata(DUOLCommon::MetaDataType::SerializeByString, true)
		, metadata(DUOLCommon::MetaDataType::MappingType, DUOLCommon::MappingType::Resource)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Mesh)
	);
}

namespace DUOLGameEngine
{
	MeshFilter::MeshFilter() :
		ComponentBase(nullptr, TEXT("MeshFilter"))
		, _mesh(nullptr)
	{
		// ���ƾƾƾƾƾƾ� !
	}

	MeshFilter::MeshFilter(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		ComponentBase(owner, name)
		, _mesh(nullptr)
	{
		MeshRenderer* meshRenderer = 
			GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>();

		if (meshRenderer != nullptr)
		{
			meshRenderer->_meshFilter = this;
		}
	}

	MeshFilter::~MeshFilter()
	{

	}
}