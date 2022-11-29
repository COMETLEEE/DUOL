#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"

#include "DUOLGameEngine/ECS/GameObject.h"

namespace DUOLGameEngine
{
	BehaviourBase::BehaviourBase(const std::weak_ptr<GameObject>& owner, const DUOLCommon::tstring& name) :
		ComponentBase(owner, name),
		_isEnabled(true),
		_isActivedAndEnabled(false)
	{

	}

	BehaviourBase::~BehaviourBase()
	{

	}

	void BehaviourBase::SetIsEnabled(bool value)
	{
		// ����� ������ ���� �߰� ������ �ʿ�� �����ϴ�.
		if (value == _isEnabled)
			return;

		const std::shared_ptr<GameObject>& gameObject = GetGameObject();

		value == true ? gameObject->SetBehaviourEnabled(this->shared_from_this())
			: gameObject->SetBehaviourDisabled(this->shared_from_this());

		// �ٲ��ݴϴ�.
		_isEnabled = value;
	}
}