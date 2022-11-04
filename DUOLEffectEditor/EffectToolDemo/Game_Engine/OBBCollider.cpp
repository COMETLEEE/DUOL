#include "pch.h"
#include "OBBCollider.h"

Muscle::OBBCollider::OBBCollider(std::shared_ptr<GameObject> _GameObject) : BoxCollider(_GameObject)
{
	m_ColliderType = ColliderType::OBB;
}

Muscle::OBBCollider::~OBBCollider()
{

}

void Muscle::OBBCollider::Finalize()
{
	__super::Finalize();
}

void Muscle::OBBCollider::ColliderCheck(std::shared_ptr<Collider> _Other)
{
	// 박스끼리 충돌 안할거 같음....
	switch (_Other->m_ColliderType)
	{
	case ColliderType::Sphere:
	{
		SphereToBox(std::dynamic_pointer_cast<SphereCollider>(_Other), this->shared_from_this());
		break;
	}
	case ColliderType::Capsule:
	{
		CapsuleToBox(std::dynamic_pointer_cast<CapsuleCollider>(_Other), this->shared_from_this());
		break;
	}
	case ColliderType::AABB:
	{
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

void Muscle::OBBCollider::UpdateMinMax()
{
	//l b b -
	//r t f +
	Vector3 size = GetSize() / 2;

	Vector3 lbf = Vector3(-size.x, -size.y, size.z);
	Vector3 lbb = Vector3(-size.x, -size.y, -size.z);
	Vector3 rbf = Vector3(size.x, -size.y, size.z);
	Vector3 ltf = Vector3(-size.x, size.y, size.z);

	lbf = Vector3::Transform(lbf, GetWorldRotTM());
	lbb = Vector3::Transform(lbb, GetWorldRotTM());
	rbf = Vector3::Transform(rbf, GetWorldRotTM());
	ltf = Vector3::Transform(ltf, GetWorldRotTM());

	_maxX = max(abs(lbf.x), abs(lbb.x));
	_maxX = max(_maxX, abs(rbf.x));
	_maxX = max(_maxX, abs(ltf.x));

	_maxZ = max(abs(lbf.z), abs(lbb.z));
	_maxZ = max(_maxX, abs(rbf.z));
	_maxZ = max(_maxX, abs(ltf.z));

	_minX = -_maxX;
	_minZ = -_maxZ;

}
