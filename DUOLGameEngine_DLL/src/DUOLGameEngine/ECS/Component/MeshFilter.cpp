#include "DUOLGameEngine/ECS/Component/MeshFilter.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::MeshFilter>("MeshFilter")
	.constructor<const std::weak_ptr<DUOLGameEngine::GameObject>&, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_mesh", &DUOLGameEngine::MeshFilter::_mesh)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::SerializeByUUID, true)
		, metadata(DUOLCommon::MetaDataType::UUIDSerializeType, DUOLCommon::UUIDSerializeType::Resource)
	);
}

namespace DUOLGameEngine
{
	MeshFilter::MeshFilter() :
		ComponentBase(std::weak_ptr<GameObject>(), TEXT("MeshFilter"))
		, _mesh(nullptr)
	{
		// 으아아아아아아아 !
	}

	MeshFilter::MeshFilter(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name) :
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