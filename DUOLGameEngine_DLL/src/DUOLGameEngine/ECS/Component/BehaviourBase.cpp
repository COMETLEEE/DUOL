#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"

#include "DUOLGameEngine/ECS/GameObject.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::BehaviourBase>("BehaviourBase")
	.constructor()
	(

	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("Enable", &DUOLGameEngine::BehaviourBase::GetIsEnabled, &DUOLGameEngine::BehaviourBase::SetIsEnabled)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	);
}

namespace DUOLGameEngine
{
	BehaviourBase::BehaviourBase() :
		ComponentBase(nullptr, TEXT("BehaviourBase"))
		, _isEnabled(true)
		, _isStarted(false)
		, _isAwaken(false)
	{

	}

	BehaviourBase::BehaviourBase(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		ComponentBase(owner, name)
		, _isEnabled(true)
		, _isStarted(false)
		, _isAwaken(false)
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

		// Awake�� Start Function�� ������� �ʾҽ��ϴ�. ù ��° Enable(true) ��� ��
		if ((value) && (!_isAwaken))
		{
			OnAwake();

			_isAwaken = true;
		}

		// Awake�� Start Function�� ������� �ʾҽ��ϴ�. ù ��° Enable(true) ��� ��
		if ((value) && (!_isStarted))
		{
			OnStart();

			_isStarted = true;
		}

		GameObject* gameObject = GetGameObject();

		if (gameObject != nullptr)
		{
			value == true ? gameObject->SetBehaviourEnabled(this->shared_from_this())
				: gameObject->SetBehaviourDisabled(this->shared_from_this());
		}

		// �ٲ��ݴϴ�.
		_isEnabled = value;
	}

	bool BehaviourBase::GetIsActiveAndEnabled() const
	{
		return GetGameObject()->_isActive && _isEnabled;
	}
}