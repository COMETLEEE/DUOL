#include "DUOLGameEngine/Util/Geometries.h"

#include "DUOLGameEngine/ECS/Component/Camera.h"

namespace DUOLGameEngine
{
	bool GeometryHelper::ViewFrustumCullingAABB(DUOLMath::Vector3& origin, DUOLMath::Vector3& extents, const Frustum& frustum)
	{
		bool rb = IsForwardPlane(origin, extents, frustum._rightFace);
		bool lb = IsForwardPlane(origin, extents, frustum._leftFace);
		bool tb = IsForwardPlane(origin, extents, frustum._topFace);
		bool bb = IsForwardPlane(origin, extents, frustum._bottomFace);
		bool nb = IsForwardPlane(origin, extents, frustum._nearFace);
		bool fb = IsForwardPlane(origin, extents, frustum._farFace);

		return (rb && lb && tb && bb && nb && fb);
	}

	void GeometryHelper::CreateFrustumFromCamera(const DUOLGraphicsEngine::Camera& camera, Frustum& outFrustum)
	{
		auto camMat = camera._viewMatrix.Invert();

		outFrustum._camRight = DUOLMath::Vector3{ camMat.m[0][0], camMat.m[0][1], camMat.m[0][2] };
		outFrustum._camUp = DUOLMath::Vector3{ camMat.m[1][0], camMat.m[1][1], camMat.m[1][2] };
		outFrustum._camLook = DUOLMath::Vector3{ camMat.m[2][0], camMat.m[2][1], camMat.m[2][2] };

		outFrustum._camLook.Normalize();
		outFrustum._camRight.Normalize();
		outFrustum._camUp.Normalize();

		outFrustum._nearFace = { outFrustum._camLook , camera._cameraPosition + outFrustum._camLook * camera._cameraNear };
		outFrustum._farFace = { -outFrustum._camLook , camera._cameraPosition + outFrustum._camLook * camera._cameraFar };

		DUOLMath::Vector3 FarVec = outFrustum._camLook * camera._cameraFar;
		float halfVertical = camera._cameraFar * tanf(camera._cameraVerticalFOV * 0.5f);
		float halfHorizon = halfVertical * camera._aspectRatio;

		DUOLMath::Vector3 rightNormal = FarVec + outFrustum._camRight * halfHorizon;
		DUOLMath::Vector3 leftNormal = FarVec - outFrustum._camRight * halfHorizon;

		rightNormal.Normalize();
		leftNormal.Normalize();

		outFrustum._rightFace = { rightNormal.Cross(outFrustum._camUp) , camera._cameraPosition };
		outFrustum._leftFace = { outFrustum._camUp.Cross(leftNormal) , camera._cameraPosition };

		DUOLMath::Vector3 upNormal = FarVec + outFrustum._camUp * halfVertical;
		DUOLMath::Vector3 downNormal = FarVec - outFrustum._camUp * halfVertical;

		upNormal.Normalize();
		downNormal.Normalize();

		//outFrustum._topFace = {  upNormal.Cross(outFrustum._camRight) , camera._cameraPosition };
		//outFrustum._bottomFace = { outFrustum._camRight.Cross(downNormal) , camera._cameraPosition };

		outFrustum._topFace = { outFrustum._camRight.Cross(upNormal) , camera._cameraPosition };
		outFrustum._bottomFace = { downNormal.Cross(outFrustum._camRight) , camera._cameraPosition };

		outFrustum._nearFace._normal.Normalize();
		outFrustum._farFace._normal.Normalize();
		outFrustum._rightFace._normal.Normalize();
		outFrustum._leftFace._normal.Normalize();
		outFrustum._topFace._normal.Normalize();
		outFrustum._bottomFace._normal.Normalize();
	}

	bool GeometryHelper::IsForwardPlane(DUOLMath::Vector3& centerPos, DUOLMath::Vector3& boundingBox, const Plane& plane)
	{
		//bounding box의 positive extent와 plane normal간의 내적. 
		float r = boundingBox.x * fabs(plane._normal.x) + boundingBox.y * fabs(plane._normal.y) + boundingBox.z * fabs(plane._normal.z);

		//signed distance
		float distPlane = plane._normal.Dot(plane._position);
		float s = plane._normal.Dot(centerPos) - distPlane;

		return -r <= s;
	}
}