#include "DUOLGameEngine/ECS/Component/ComponentBase.h"
#include "DUOLGameEngine/ECS/GameObject.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::ComponentBase>("ComponentBase")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_owner", &DUOLGameEngine::ComponentBase::_owner)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::SerializeByUUID, true)
		, metadata(DUOLCommon::MetaDataType::MappingType, DUOLCommon::MappingType::FileUUID)
	);
}

namespace DUOLGameEngine
{
	ComponentBase::ComponentBase() :
		ObjectBase(TEXT("Component"), ObjectType::Component)
		, _owner(nullptr)
	{
	}

	ComponentBase::ComponentBase(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		ObjectBase(name, ObjectType::Component)
		, _owner(owner)
	{
		// Transform Component´Â ºÐ¸í _owner.lock()->GetTransform() ÀÌ ¾øÀ»ÅÙµ­ 
	}

	ComponentBase::~ComponentBase()
	{
	}
	 
	Transform* ComponentBase::GetTransform() const
	{
		assert(_owner != nullptr);

		return _owner->GetTransform();
	}

	const DUOLCommon::tstring& ComponentBase::GetTag() const
	{
		assert(_owner != nullptr);

		return _owner->GetTag();
	}

	bool ComponentBase::CompareTag(const DUOLCommon::tstring& tag) const
	{
		assert(_owner != nullptr);

		return (tag == _owner->GetTag());
	}
}