#include "OBBCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"

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
	DUOLMath::Vector3 size = GetSize() / 2;

	DUOLMath::Vector3 lbf = DUOLMath::Vector3(-size.x, -size.y, size.z);
	DUOLMath::Vector3 lbb = DUOLMath::Vector3(-size.x, -size.y, -size.z);
	DUOLMath::Vector3 rbf = DUOLMath::Vector3(size.x, -size.y, size.z);
	DUOLMath::Vector3 ltf = DUOLMath::Vector3(-size.x, size.y, size.z);

	lbf = DUOLMath::Vector3::Transform(lbf, GetWorldRotTM());
	lbb = DUOLMath::Vector3::Transform(lbb, GetWorldRotTM());
	rbf = DUOLMath::Vector3::Transform(rbf, GetWorldRotTM());
	ltf = DUOLMath::Vector3::Transform(ltf, GetWorldRotTM());

	_maxX = max(abs(lbf.x), abs(lbb.x));
	_maxX = max(_maxX, abs(rbf.x));
	_maxX = max(_maxX, abs(ltf.x));

	_maxZ = max(abs(lbf.z), abs(lbb.z));
	_maxZ = max(_maxX, abs(rbf.z));
	_maxZ = max(_maxX, abs(ltf.z));

	_minX = -_maxX;
	_minZ = -_maxZ;

}
