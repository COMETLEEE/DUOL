#include "DUOLGameEngine/ECS/Component/ComponentBase.h"
#include "DUOLGameEngine/ECS/GameObject.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::ComponentBase>("ComponentBase")
	.constructor<const std::weak_ptr<DUOLGameEngine::GameObject>&, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

namespace DUOLGameEngine
{
	ComponentBase::ComponentBase(const std::weak_ptr<GameObject>& owner, const DUOLCommon::tstring& name) :
		ObjectBase(name, ObjectType::Component)
		, _owner(owner)
	{
		// Transform Component¥¬ ∫–∏Ì _owner.lock()->GetTransform() ¿Ã æ¯¿ª≈Ÿµ≠ 
	}

	ComponentBase::~ComponentBase()
	{
		if (!_owner.expired())
			_owner.reset();
	}

	Transform* ComponentBase::GetTransform() const
	{
		std::shared_ptr<GameObject> owner = _owner.lock();

		assert(owner != nullptr);

		return owner->GetTransform();
	}

	const DUOLCommon::tstring& ComponentBase::GetTag() const
	{
		const std::shared_ptr<GameObject> owner = _owner.lock();

		assert(owner != nullptr);

		return owner->GetTag();
	}

	bool ComponentBase::CompareTag(const DUOLCommon::tstring& tag) const
	{
		const std::shared_ptr<GameObject> owner = _owner.lock();

		assert(owner != nullptr);

		return (tag == owner->GetTag());
	}
}
