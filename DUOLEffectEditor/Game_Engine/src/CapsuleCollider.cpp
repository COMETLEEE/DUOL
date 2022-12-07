#include "CapsuleCollider.h"
#include "Transform.h"
#include "OBBCollider.h"
#include "SphereCollider.h"

Muscle::CapsuleCollider::CapsuleCollider(std::shared_ptr<GameObject> _GameObject) : Collider(_GameObject)
{
	m_ColliderType = ColliderType::Capsule;
}

Muscle::CapsuleCollider::~CapsuleCollider()
{

}
void Muscle::CapsuleCollider::ColliderCheck(std::shared_ptr<Collider> _Other)
{
	switch (_Other->m_ColliderType)
	{
	case ColliderType::Sphere:
	{
		std::shared_ptr<SphereCollider> othertemp = std::dynamic_pointer_cast<SphereCollider>(_Other);

		CapsuleToSphere(this->shared_from_this(), othertemp);

		break;
	}
	case ColliderType::AABB:
	{

		break;
	}
	case ColliderType::OBB:
	{
		CapsuleToBox(this->shared_from_this(), std::dynamic_pointer_cast<OBBCollider>(_Other));
		break;
	}
	case ColliderType::Capsule:
	{
		std::shared_ptr<CapsuleCollider> othertemp = std::dynamic_pointer_cast<CapsuleCollider>(_Other);

		CapsuleToCapsule(this->shared_from_this(), othertemp);
		break;
	}
	default:
		break;
	}
}

void Muscle::CapsuleCollider::UpdateMinMax()
{
	auto topMax = GetTopPosition() + DUOLMath::Vector3(GetRadius());
	auto topMin = GetTopPosition() - DUOLMath::Vector3(GetRadius());

	auto bottomMax = GetBottomPosition() + DUOLMath::Vector3(GetRadius());
	auto bottomMin = GetBottomPosition() - DUOLMath::Vector3(GetRadius());
	_minX = min(topMin.x, bottomMin.x);
	_maxX = max(topMax.x, bottomMax.x);
	_minZ = min(topMin.z, bottomMin.z);
	_maxZ = max(topMax.z, bottomMax.z);
}

float Muscle::CapsuleCollider::DistLinetoPoint(DUOLMath::Vector3 point)
{
	DUOLMath::Vector3 start = GetWorldTopPosition();
	DUOLMath::Vector3 end = GetWorldBottomPosition();

	DUOLMath::Vector3 a = end - start; // 캡슐의 중심 벡터
	a.Normalize();
	DUOLMath::Vector3 b = point - start; // 캡슐의 시작 점에서 타겟까지의 벡터.

	float dot = a.Dot(b);

	if (dot < 0)
		return b.Length();

	a = a * dot; // b를 a에 투영 proj_b


	if (a.Length() > GetDistance()) // 투영 b의 길이가 distance보다 길다면 끝점과 더 가까운것이다. 끝점에서 다시 연산하자!
	{
		a = start - end; // 캡슐의 중심 벡터
		a.Normalize();
		b = point - end; // 캡슐의 시작 점에서 타겟까지의 벡터.


		dot = a.Dot(b);

		if (dot < 0)
			return b.Length();
		a = a * dot; // b를 a에 투영 proj_b
	}

	DUOLMath::Vector3 D = b - a; // 수직으로 타겟을 가르키는 벡터.


	return D.Length();
}

void Muscle::CapsuleCollider::SetCapsule(float _Radius, float _Distnace)
{
	m_Radius = _Radius;
	m_Distance = _Distnace;
}

const float Muscle::CapsuleCollider::GetRadius()
{
	return m_Radius * m_Transform->GetWorldScale().x;
}

const float Muscle::CapsuleCollider::GetDistance()
{
	return m_Distance * m_Transform->GetWorldScale().y;
}

const DUOLMath::Vector3 Muscle::CapsuleCollider::GetTopPosition()
{
	DUOLMath::Vector3 topPos = DUOLMath::Vector3::Zero;
	topPos.y += GetDistance() / 2;
	// 로컬 좌표의 포지션을 반환!
	return DUOLMath::Vector3::Transform(topPos, GetWorldRotTM());
}

const DUOLMath::Vector3 Muscle::CapsuleCollider::GetBottomPosition()
{
	DUOLMath::Vector3 topPos = DUOLMath::Vector3::Zero;
	topPos.y -= GetDistance() / 2;
	// 로컬 좌표의 포지션을 반환!
	return DUOLMath::Vector3::Transform(topPos, GetWorldRotTM());
}

const DUOLMath::Vector3 Muscle::CapsuleCollider::GetWorldTopPosition()
{
	return  GetTopPosition() + GetWorldPosition();
}

const DUOLMath::Vector3 Muscle::CapsuleCollider::GetWorldBottomPosition()
{
	return GetBottomPosition() + GetWorldPosition();
}

const DUOLMath::Matrix Muscle::CapsuleCollider::GetWorldRotTM()
{
	return m_Transform->GetWorldRotateTM();
}

void Muscle::CapsuleCollider::Finalize()
{
	__super::Finalize();
}