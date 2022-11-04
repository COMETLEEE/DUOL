#include "pch.h"
#include "Collider.h"
namespace Muscle
{

	UINT Collider::idCount = 0;

	Collider::Collider(std::shared_ptr<GameObject> _GameObject) : IComponents(_GameObject), m_isCollided(false), m_LocalPosition(Vector3::Zero),
		_minX(0), _maxX(0), _minZ(0), _maxZ(0), _isColliderIsEnable(true)
	{
		m_Transform = _GameObject->GetTransform();
		_ColliderID = idCount++;
	}

	Collider::~Collider()
	{
		for (auto& col : m_CurColliders)
			col.second.reset();

		for (auto& col : m_PrevColliders)
			col.second.reset();

		m_CurColliders.clear();

		m_PrevColliders.clear();
	}

	void Collider::Update()
	{
		UpdateMinMax();
		//for (auto& iter : m_CurColliders)
		//{
		//	StayEvent(iter.second);
		//}
	}


	Vector3 Collider::GetWorldPosition()
	{
		return m_Transform->GetWorldPosition() + Vector3::Transform(m_LocalPosition, m_Transform->GetWorldRotateTM());
	}

	Vector3 Collider::GetLocalPosition()
	{
		return m_LocalPosition;
	}

	void Collider::SetLocalPostion(Vector3 local)
	{
		m_LocalPosition = local;
	}

	void Collider::Finalize()
	{
		for (auto col : m_CurColliders)
			col.second.reset();

		for (auto col : m_PrevColliders)
			col.second.reset();
	}

	void Collider::EnterEvent(std::shared_ptr<Collider> _Other)
	{
		const auto& check = m_PrevColliders.find(_Other->_ColliderID);

		m_CurColliders.insert({ _Other->_ColliderID,_Other });

		if (m_PrevColliders.end() == check)
			GetGameObject()->OnEnterCollision(_Other);
	}

	void Collider::StayEvent(std::shared_ptr<Collider> _Other)
	{
		GetGameObject()->OnStayCollision(_Other);
		m_isCollided = true;
	}

	void Collider::ExitEvent(std::shared_ptr<Collider> _Other)
	{
		if (GetGameObject())
			GetGameObject()->OnExitCollision(_Other);
	}


	void Collider::CapsuleToSphere(std::shared_ptr<CapsuleCollider> capsule, std::shared_ptr<SphereCollider> sphere)
	{
		float radiusPlus = capsule->GetRadius() + sphere->GetRadius(); // 캡슐과 구의 반지름 합.
		if (radiusPlus > capsule->DistLinetoPoint(sphere->GetWorldPosition()))// 캡슐 중앙 벡터와 점의 거리.
		{
			capsule->EnterEvent(sphere);
			sphere->EnterEvent(capsule);
		}
	}

	void Collider::AABBToAABB(std::shared_ptr<AABBCollider> aabb1, std::shared_ptr<AABBCollider> aabb2)
	{
		// 쓸 일 없을듯??
	}

	void Collider::SphereToSphere(std::shared_ptr<SphereCollider> sphere1, std::shared_ptr<SphereCollider> sphere2)
	{
		const Vector3& _thispos = sphere1->GetWorldPosition();
		const Vector3& _otherpos = sphere2->GetWorldPosition();

		Vector3 _Distance = _thispos - _otherpos;

		_Distance = XMVector3Length(_Distance);

		float _RadiusPlus = sphere2->GetRadius() + sphere1->GetRadius();

		if (_RadiusPlus > _Distance.x) // 충돌.
		{

			sphere1->EnterEvent(sphere2);
			sphere2->EnterEvent(sphere1);

		}


	}

	void Collider::CapsuleToCapsule(std::shared_ptr<CapsuleCollider> capsule1, std::shared_ptr<CapsuleCollider> capsule2)
	{
		Vector3 tp1, bp1, tp2, bp2;
		float radiusPlus = capsule1->GetRadius() + capsule2->GetRadius(); // 캡슐과 구의 반지름 합.

		tp1 = capsule1->GetWorldTopPosition();
		bp1 = capsule1->GetWorldBottomPosition();

		tp2 = capsule2->GetWorldTopPosition();
		bp2 = capsule2->GetWorldBottomPosition();

		float a, b, c, d;
		auto EventCheck = [&]() -> bool {for (auto& iter : capsule1->m_CurColliders) { if (iter.second == capsule2) { return false; } }return true; }; // 이미 충돌중이라면 거른다..
		auto RadiusCheck = [&](float dist)->bool {if (dist < radiusPlus)return true; else return false; }; // 거리를 입력받아 반지름과 비교하는 함수.
		auto EventSet = [&](bool _isBool)->bool {
			if (_isBool)
			{
				capsule1->EnterEvent(capsule2);
				capsule2->EnterEvent(capsule1);
			}
			return _isBool;
		};
		// 여기서 부터 하시면 됩니다!!
		// 아래 코드 최적화 및, 직선 충돌 하시면 됩니당!!


		if (EventSet(RadiusCheck(capsule1->DistLinetoPoint(tp2))))
			return;
		else if (EventSet(RadiusCheck(capsule1->DistLinetoPoint(bp2))))
			return;
		else if (EventSet(RadiusCheck(capsule2->DistLinetoPoint(tp1))))
			return;
		else if (EventSet(RadiusCheck(capsule2->DistLinetoPoint(bp1))))
			return;
		int i = (capsule1->GetDistance() - capsule1->GetRadius() * 2) / (capsule1->GetRadius() * 2);
		int j = (capsule2->GetDistance() - capsule2->GetRadius() * 2) / (capsule2->GetRadius() * 2);

		if (i > 0)
		{
			Vector3 direction = capsule2->GetWorldPosition() - tp2;
			direction.Normalize();

			float diameter = capsule2->GetRadius() * 2;

			direction = diameter * direction;

			Vector3 start = tp2;
			for (int ii = 0; ii < i; ii++)
			{
				start += direction;
				if (EventSet(RadiusCheck(capsule1->DistLinetoPoint(start))))
					return;
			}
		}

		if (j > 0)
		{
			Vector3 direction = capsule1->GetWorldPosition() - tp1;
			direction.Normalize();

			float diameter = capsule1->GetRadius() * 2;

			direction = diameter * direction;

			Vector3 start = tp1;
			for (int ii = 0; ii < j; ii++)
			{
				start += direction;
				if (EventSet(RadiusCheck(capsule2->DistLinetoPoint(start))))
					return;
			}
		}
	}

	void Collider::SphereToBox(std::shared_ptr<SphereCollider> sphere, std::shared_ptr<BoxCollider> box)
	{
		// 구의 중심에 OBB 회전량의 역변환을 가하면 된다.
		Matrix boxRotInverse = box->GetWorldRotTM().Invert();
		Vector3 sphereLocalCenter = Vector3::Transform(sphere->GetWorldPosition(), boxRotInverse);

		if (PointToBoxDistance(sphereLocalCenter, box) > sphere->GetRadius())
		{
			//충돌안함

		}
		else
		{
			//충돌함
			sphere->EnterEvent(box);
			box->EnterEvent(sphere);
		}
	}

	void Collider::CapsuleToBox(std::shared_ptr<CapsuleCollider> capsule, std::shared_ptr<BoxCollider> box)
	{
		Vector3 tp, bp;
		tp = capsule->GetWorldTopPosition();
		bp = capsule->GetWorldBottomPosition();

		Matrix boxRotInverse = box->GetWorldRotTM().Invert();

		if (PointToBoxDistance(Vector3::Transform(tp, boxRotInverse), box) > capsule->GetRadius())
		{
			//충돌안함

		}
		else
		{
			//충돌함

			capsule->EnterEvent(box);
			box->EnterEvent(capsule);
			return;
		}
		if (PointToBoxDistance(Vector3::Transform(bp, boxRotInverse), box) > capsule->GetRadius())
		{
			//충돌안함

		}
		else
		{
			//충돌함
			capsule->EnterEvent(box);
			box->EnterEvent(capsule);
			return;
		}




		int i = (capsule->GetDistance() - capsule->GetRadius() * 2) / (capsule->GetRadius() * 2);
		if (i > 0)
		{
			Vector3 direction = capsule->GetWorldPosition() - tp;
			direction.Normalize();

			float diameter = capsule->GetRadius() * 2;

			direction = diameter * direction;

			Vector3 start = tp;
			for (int ii = 0; ii < i; ii++)
			{
				start += direction;
				if (PointToBoxDistance(Vector3::Transform(start, boxRotInverse), box) > capsule->GetRadius())
				{
					//충돌안함

				}
				else
				{
					//충돌함

					capsule->EnterEvent(box);
					box->EnterEvent(capsule);

					return;
				}
			}
		}


	}


	float Collider::PointToBoxDistance(Vector3 pointPos, std::shared_ptr<BoxCollider> box)
	{
		//OBB to Box
		//AABB to AABB 의 확장판!

		Matrix boxRotInverse = box->GetWorldRotTM().Invert();

		// Box의 World Center도 이동시켜서 같은 스페이스로 바꿔준다.
		Vector3 boxLocalCenter = Vector3::Transform(box->GetWorldPosition(), boxRotInverse);

		// AABB V.S Sphere
		Vector3 boxMin = std::move(boxLocalCenter - box->GetSize() * 0.5f); // 박스의 최소점

		Vector3 boxMax = std::move(boxLocalCenter + box->GetSize() * 0.5f); // 박스의 최대점.

		// aabb v.s aabb와 같음!
		float minX = std::max<float>(boxMin.x, std::min<float>(pointPos.x, boxMax.x));
		float minY = std::max<float>(boxMin.y, std::min<float>(pointPos.y, boxMax.y));
		float minZ = std::max<float>(boxMin.z, std::min<float>(pointPos.z, boxMax.z));

		float dist = (Vector3(minX, minY, minZ) - pointPos).Length();
		return dist;
	}


}