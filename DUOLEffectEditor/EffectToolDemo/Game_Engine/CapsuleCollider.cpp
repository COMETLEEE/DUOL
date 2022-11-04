#include "pch.h"
#include "CapsuleCollider.h"

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
	auto topMax = GetTopPosition() + Vector3(GetRadius());
	auto topMin = GetTopPosition() - Vector3(GetRadius());

	auto bottomMax = GetBottomPosition() + Vector3(GetRadius());
	auto bottomMin = GetBottomPosition() - Vector3(GetRadius());
	_minX = min(topMin.x, bottomMin.x);
	_maxX = max(topMax.x, bottomMax.x);
	_minZ = min(topMin.z, bottomMin.z);
	_maxZ = max(topMax.z, bottomMax.z);
}

float Muscle::CapsuleCollider::DistLinetoPoint(Vector3 point)
{
	Vector3 start = GetWorldTopPosition();
	Vector3 end = GetWorldBottomPosition();

	Vector3 a = end - start; // ĸ���� �߽� ����
	a.Normalize();
	Vector3 b = point - start; // ĸ���� ���� ������ Ÿ�ٱ����� ����.

	float dot = a.Dot(b);

	if (dot < 0)
		return b.Length();

	a = a * dot; // b�� a�� ���� proj_b


	if (a.Length() > GetDistance()) // ���� b�� ���̰� distance���� ��ٸ� ������ �� �������̴�. �������� �ٽ� ��������!
	{
		a = start - end; // ĸ���� �߽� ����
		a.Normalize();
		b = point - end; // ĸ���� ���� ������ Ÿ�ٱ����� ����.


		dot = a.Dot(b);

		if (dot < 0)
			return b.Length();
		a = a * dot; // b�� a�� ���� proj_b
	}

	Vector3 D = b - a; // �������� Ÿ���� ����Ű�� ����.


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

const DirectX::SimpleMath::Vector3 Muscle::CapsuleCollider::GetTopPosition()
{
	Vector3 topPos = Vector3::Zero;
	topPos.y += GetDistance() / 2;
	// ���� ��ǥ�� �������� ��ȯ!
	return Vector3::Transform(topPos, GetWorldRotTM());
}

const DirectX::SimpleMath::Vector3 Muscle::CapsuleCollider::GetBottomPosition()
{
	Vector3 topPos = Vector3::Zero;
	topPos.y -= GetDistance() / 2;
	// ���� ��ǥ�� �������� ��ȯ!
	return Vector3::Transform(topPos, GetWorldRotTM());
}

const DirectX::SimpleMath::Vector3 Muscle::CapsuleCollider::GetWorldTopPosition()
{
	return  GetTopPosition() + GetWorldPosition();
}

const DirectX::SimpleMath::Vector3 Muscle::CapsuleCollider::GetWorldBottomPosition()
{
	return GetBottomPosition() + GetWorldPosition();
}

const DirectX::SimpleMath::Matrix Muscle::CapsuleCollider::GetWorldRotTM()
{
	return m_Transform->GetWorldRotateTM();
}

void Muscle::CapsuleCollider::Finalize()
{
	__super::Finalize();
}