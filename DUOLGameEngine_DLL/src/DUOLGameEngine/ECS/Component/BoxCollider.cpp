#include "DUOLGameEngine/ECS/Component/BoxCollider.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/PhysicsManager.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::BoxCollider>("BoxCollider")
	.constructor<const std::weak_ptr<DUOLGameEngine::GameObject>&, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

namespace DUOLGameEngine
{
	BoxCollider::BoxCollider(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name) :
		ColliderBase(owner, name)
		, _physicsBox()
		// , _center(DUOLMath::Vector3::Zero)
		, _center(DUOLMath::Vector3::Up * 10.f)
		, _size (DUOLMath::Vector3(20.f, 20.f, 20.f))
	{
		
	}

	BoxCollider::~BoxCollider()
	{
		_physicsBox.lock()->Release();

		_physicsBox.reset();
	}

	void BoxCollider::OnEnable()
	{
		// ���δ�.
		_physicsActor.lock()->AttachShape(_physicsBox);
	}

	void BoxCollider::OnDisable()
	{
		// ����.
		_physicsActor.lock()->DetachShape(_physicsBox);
	}

	void BoxCollider::SetCenter(const DUOLMath::Vector3& center)
	{
		// TODO : Shape�� SetLocalPose ������ �� �ֵ��� �����������.
		_center = center;

		// ���� ���� ���ݾ� .. Physics Box ���� �Ŀ� �����ϵ��� ����������մϴ�.
		_physicsBox.lock()->SetLocalPose(center);
	}

	void BoxCollider::SetSize(const DUOLMath::Vector3& size)
	{
		// TODO : Shape .. PhysicsBox ����������մϴ�.
		_size = size;

		// ���� ���� ���ݾ� .. Physics Box ���� �Ŀ� �����ϵ��� ����������մϴ�.
		_physicsBox.lock()->SetScale(size.x / 2.f, size.y / 2.f, size.z / 2.f);
	}
}