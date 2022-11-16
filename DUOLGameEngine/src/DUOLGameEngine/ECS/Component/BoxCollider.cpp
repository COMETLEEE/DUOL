#include "DUOLGameEngine/ECS/Component/BoxCollider.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/PhysicsManager.h"

namespace DUOLGameEngine
{
	BoxCollider::BoxCollider(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name) :
		ColliderBase(owner, name)
		, _physicsBox()
		, _center(DUOLMath::Vector3::Zero)
		, _size (DUOLMath::Vector3(3.f, 3.f, 3.f))
	{
		
	}

	BoxCollider::~BoxCollider()
	{
		_physicsBox.reset();

		_physicsBox.lock()->Release();
	}

	void BoxCollider::OnEnable()
	{
		// 붙인다.
		_physicsActor.lock()->AttachShape(_physicsBox);
	}

	void BoxCollider::OnDisable()
	{
		// 땐다.
		_physicsActor.lock()->DetachShape(_physicsBox);
	}

	void BoxCollider::SetCenter(const DUOLMath::Vector3& center)
	{
		// TODO : SetLocalPose 감안할 수 있도록 설정해줘야함.
	}

	void BoxCollider::SetSize(const DUOLMath::Vector3& size)
	{
		// TODO : Shape .. PhysicsBox 변경해줘야합니다.
	}
}