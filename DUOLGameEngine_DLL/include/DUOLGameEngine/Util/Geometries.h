#pragma once
#include "DUOLMath/DUOLMath.h"

namespace DUOLGraphicsEngine
{
	struct Camera;
}

namespace DUOLGameEngine
{
	struct Plane
	{
		Plane() :
			_normal(0.f, 1.f, 0.f)
			, _position(0.f)
		{
		}

		Plane(DUOLMath::Vector3 normal, DUOLMath::Vector3 pos) :
			_normal(normal)
			, _position(pos)
		{
		}

		DUOLMath::Vector3 _normal;

		DUOLMath::Vector3 _position;
	};

	struct Frustum
	{
		Frustum() :
			_topFace()
			, _bottomFace()
			, _rightFace()
			, _leftFace()
			, _farFace()
			, _nearFace()
		{
		}

		Plane _topFace;

		Plane _bottomFace;

		Plane _rightFace;

		Plane _leftFace;

		Plane _farFace;

		Plane _nearFace;

		DUOLMath::Vector3 _camLook;

		DUOLMath::Vector3 _camUp;

		DUOLMath::Vector3 _camRight;
	};

	class GeometryHelper
	{
	public:
		static bool ViewFrustumCullingAABB(DUOLMath::Vector3& origin, DUOLMath::Vector3& extents, const Frustum& frustum);

		static void CreateFrustumFromCamera(const DUOLGraphicsEngine::Camera& camera, Frustum& outFrustum);

	private:
		static bool IsForwardPlane(DUOLMath::Vector3& centerPos, DUOLMath::Vector3& boundingBox, const Plane& plane);
	};
}