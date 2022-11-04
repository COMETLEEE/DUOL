#pragma once
#include "SimpleMath.h"
#include "SimpleMath.inl"
//using namespace DirectX::SimpleMath;
namespace Muscle
{
	// 바운딩 큐브
	struct BoundingCube
	{
		BoundingCube(const Vector3& localScale, const Vector3& localCenter, const Matrix& localMatrix) :
			_localScale(localScale), _localCenter(localCenter), _localMatrix(localMatrix)
		{
			float halfX = _localScale.x / 2;
			float halfY = _localScale.y / 2;
			float halfZ = _localScale.z / 2;

			_localPoint[0] = _localCenter + Vector3(-halfX, -halfY, -halfZ);
			_localPoint[1] = _localCenter + Vector3(-halfX, halfY, -halfZ);
			_localPoint[2] = _localCenter + Vector3(halfX, -halfY, -halfZ);
			_localPoint[3] = _localCenter + Vector3(-halfX, -halfY, halfZ);
			_localPoint[4] = _localCenter + Vector3(halfX, -halfY, halfZ);
			_localPoint[5] = _localCenter + Vector3(halfX, halfY, -halfZ);
			_localPoint[6] = _localCenter + Vector3(halfX, halfY, halfZ);
			_localPoint[7] = _localCenter + Vector3(-halfX, halfY, halfZ);
		}

		Vector3 _localScale;

		Vector3 _localCenter;

		Matrix _localMatrix;

		Vector3 _localPoint[8];
	};

	// 바운딩 렉트
	struct BoundingRect
	{
		BoundingRect(Vector2 leftTop, Vector2 rightBottom) : _leftTop(leftTop), _rightBottom(rightBottom)
		{
		}

		BoundingRect(Vector4 rect) : _leftTop(Vector2(rect.x, rect.y)), _rightBottom(Vector2(rect.z, rect.w))
		{
		}

		Vector2 _leftTop;

		Vector2 _rightBottom;
	};

	// 평면
	struct Plane
	{
		Plane() : _normal(Vector3::Zero), _distance(0)
		{}

		// 반시계 방향으로 감기는 평면을 생성합니다.
		Plane(Vector3 first, Vector3 second, Vector3 third)
		{
			Vector3 e1 = first - second;

			Vector3 e2 = first - third;

			_normal = e1.Cross(e2);

			_normal.Normalize();

			_distance = -_normal.Dot(first);

			_point = third;
		}


		Vector3 _normal;

		Vector3 _point;

		float _distance;
	};

	// 바운딩 프러스텀 .. 평면 6개로 구성되어 있다.
	struct BoundingFrustum
	{
		Plane _planes[6];

		// Far Plane Corner => Left Top, Left Bottom, Right Bottom, Right Top
		Vector4 _frustumCorner[4];
	};
}
