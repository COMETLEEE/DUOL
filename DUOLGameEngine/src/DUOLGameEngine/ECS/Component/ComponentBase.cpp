#include "DUOLGameEngine/ECS/Component/ComponentBase.h"

namespace DUOLGameEngine
{
	ComponentBase::ComponentBase(std::shared_ptr<GameObject> owner, const DUOLCommon::tstring& name) :
		ObjectBase(name)
		, _owner(std::move(owner))
	{

	}

	ComponentBase::~ComponentBase()
	{
		_transform.reset();

		_owner.reset();
	}
}