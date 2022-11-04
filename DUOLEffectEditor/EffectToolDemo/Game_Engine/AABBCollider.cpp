#include "pch.h"
#include "AABBCollider.h"

namespace Muscle
{
	AABBCollider::AABBCollider(std::shared_ptr<GameObject> _GameObject) :BoxCollider(_GameObject)
	{
		m_ColliderType = ColliderType::AABB;
	}

	AABBCollider::~AABBCollider()
	{
	}


	void AABBCollider::Finalize()
	{
		__super::Finalize();
	}

	void AABBCollider::ColliderCheck(std::shared_ptr<Collider> _Other)
	{
		switch (_Other->m_ColliderType)
		{
		case ColliderType::Sphere:
		{
			break;
		}
		case ColliderType::AABB:
		{
			std::shared_ptr<AABBCollider> _OtherCollider = std::dynamic_pointer_cast<AABBCollider>(_Other);
			AABBToAABB(this->shared_from_this(), _OtherCollider);

			break;
		}
		case ColliderType::OBB:
		{

			break;
		}
		default:
			break;
		}
	}

	void AABBCollider::Start()
	{
	}

	void AABBCollider::Render()
	{
	}
	void AABBCollider::UpdateMinMax()
	{
		_minX = -GetSize().x;
		_maxX = GetSize().x;
		_minZ = -GetSize().z;
		_maxZ = GetSize().z;

	}
}