#include "pch.h"
#include "SphereCollider.h"

namespace Muscle
{
	SphereCollider::SphereCollider(std::shared_ptr<GameObject> _GameObject) :Collider(_GameObject), m_Radius(0)
	{
		m_ColliderType = ColliderType::Sphere;
	}

	SphereCollider::~SphereCollider()
	{
	}

	void SphereCollider::UpdateMinMax()
	{
		_minX = -GetRadius();
		_maxX = GetRadius();
		_minZ = -GetRadius();
		_maxZ = GetRadius();
	}

	void SphereCollider::SetRadius(float _Radius)
	{
		m_Radius = _Radius;
	}


	const float SphereCollider::GetRadius()
	{
		return m_Radius * m_Transform->GetWorldScale().x;
	}

	void SphereCollider::Finalize()
	{
		__super::Finalize();
	}

	void SphereCollider::ColliderCheck(std::shared_ptr<Collider> _Other)
	{
		switch (_Other->m_ColliderType)
		{
		case ColliderType::Sphere:
		{

			std::shared_ptr<SphereCollider> _OtherSphere = std::dynamic_pointer_cast<SphereCollider>(_Other);
			SphereToSphere(this->shared_from_this(), _OtherSphere);

			break;
		}
		case ColliderType::AABB:
		{

			break;
		}
		case ColliderType::OBB:
		{
			SphereToBox(this->shared_from_this(), std::dynamic_pointer_cast<OBBCollider>(_Other));
			break;
		}
		case ColliderType::Capsule:
		{
			std::shared_ptr<CapsuleCollider> othertemp = std::dynamic_pointer_cast<CapsuleCollider>(_Other);
			CapsuleToSphere(othertemp, this->shared_from_this());

			break;
		}
		default:
			break;
		}
	}


}