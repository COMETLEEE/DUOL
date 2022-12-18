#include "DUOLGameEngine/ECS/Component/ComponentBase.h"
#include "DUOLGameEngine/ECS/GameObject.h"

namespace DUOLGameEngine
{
	ComponentBase::ComponentBase(const std::weak_ptr<GameObject>& owner, const DUOLCommon::tstring& name) :
		ObjectBase(name)
		, _owner(owner)
	{
		// Transform Component¥¬ ∫–∏Ì _owner.lock()->GetTransform() ¿Ã æ¯¿ª≈Ÿµ≠ 
	}

	ComponentBase::~ComponentBase()
	{
		if (!_owner.expired())
			_owner.reset();
	}

	Transform* ComponentBase::GetTransform()
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
