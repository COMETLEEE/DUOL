#include "DUOLGameEngine/ECS/Component/ComponentBase.h"
#include "DUOLGameEngine/ECS/GameObject.h"

namespace DUOLGameEngine
{
	ComponentBase::ComponentBase(const std::weak_ptr<GameObject>& owner, const DUOLCommon::tstring& name) :
		ObjectBase(name)
		, _owner(owner)
		, _transform(_owner.lock()->GetTransform())
	{

	}

	ComponentBase::~ComponentBase()
	{
		_transform.reset();

		_owner.reset();
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
