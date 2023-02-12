#include "DUOLGameEngine/ECS/Component/BoxCollider.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/PhysicsManager.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::BoxCollider>("BoxCollider")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_center", &DUOLGameEngine::BoxCollider::GetCenter, &DUOLGameEngine::BoxCollider::SetCenter)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float3)
	)
	.property("_size", &DUOLGameEngine::BoxCollider::GetSize, &DUOLGameEngine::BoxCollider::SetSize)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float3)
	);
}

namespace DUOLGameEngine
{
	BoxCollider::BoxCollider(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		ColliderBase(owner, name)
		, _physicsBox()
		// , _center(DUOLMath::Vector3::Zero)
		, _center(DUOLMath::Vector3::Up * 10.f)
		, _size (DUOLMath::Vector3(20.f, 20.f, 20.f))
	{
		
	}

	BoxCollider::~BoxCollider()
	{
		_physicsBox.reset();
	}

	void BoxCollider::OnEnable()
	{
		// 붙인다.
		if (!_physicsActor.expired())
			_physicsActor.lock()->AttachShape(_physicsBox);
	}

	void BoxCollider::OnDisable()
	{
		// 땐다.
		if (!_physicsActor.expired())
			_physicsActor.lock()->DetachShape(_physicsBox);
	}

	void BoxCollider::SetCenter(const DUOLMath::Vector3& center)
	{
		// TODO : Shape의 SetLocalPose 감안할 수 있도록 설정해줘야함.
		_center = center;

		// 없을 수도 있잖아 .. Physics Box 생성 후에 동작하도록 개선해줘야합니다.
		if (!_physicsBox.expired())
			_physicsBox.lock()->SetLocalPose(center);
	}

	void BoxCollider::SetSize(const DUOLMath::Vector3& size)
	{
		// TODO : Shape .. PhysicsBox 변경해줘야합니다.
		_size = size;

		// 없을 수도 있잖아 .. Physics Box 생성 후에 동작하도록 개선해줘야합니다.
		if (!_physicsBox.expired())
			_physicsBox.lock()->SetScale(size.x / 2.f, size.y / 2.f, size.z / 2.f);
	}
}