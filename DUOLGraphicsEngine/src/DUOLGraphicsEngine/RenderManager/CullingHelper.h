#pragma once
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderConstantBuffer.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLGraphicsEngine
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
	};


	class CullingHelper
	{
		bool ViewFrustumCulling(DUOLMath::Vector3& pos, DUOLMath::Vector3& boundingBox, const Frustum& camera);

		void CreateFrustumFromCamera(const Camera& camera, Frustum& outFrustum);

	private:
		bool IsForwardPlane(DUOLMath::Vector3& pos, DUOLMath::Vector3& boundingBox, const Frustum& camera);
	};
}
