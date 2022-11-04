#include "pch.h"
#include "IComponents.h"

namespace Muscle
{
	IComponents::IComponents(std::shared_ptr<GameObject> _GameObj) :
		_isEnable(true)
	{
		m_GameObject = _GameObj;
	}

	IComponents::~IComponents()
	{
		m_GameObject.reset();
	}

	std::shared_ptr<GameObject> IComponents::GetGameObject()
	{
		return m_GameObject.lock();
	}

	bool IComponents::GetIsEnable()
	{
		if (!GetGameObject()->GetIsEnable())
			return false;
		return _isEnable;
	}

}