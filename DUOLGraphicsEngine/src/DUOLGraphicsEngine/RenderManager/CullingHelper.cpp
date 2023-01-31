#include "CullingHelper.h"

bool DUOLGraphicsEngine::CullingHelper::ViewFrustumCulling(DUOLMath::Vector3& pos, DUOLMath::Vector3& boundingBox, const Frustum& frustum)
{

	return false;
}

bool DUOLGraphicsEngine::CullingHelper::IsForwardPlane(DUOLMath::Vector3& pos, DUOLMath::Vector3& boundingBox,
	const Frustum& camera)
{
	


	return false;
}

void DUOLGraphicsEngine::CullingHelper::CreateFrustumFromCamera(const Camera& camera, Frustum& outFrustum)
{
	DUOLMath::Vector3 camLookVec = {camera._viewMatrix.m[2][0], camera._viewMatrix.m[2][1], camera._viewMatrix.m[2][2]};
	DUOLMath::Vector3 camRightVec = { camera._viewMatrix.m[0][0], camera._viewMatrix.m[0][1], camera._viewMatrix.m[0][2] };
	DUOLMath::Vector3 camUpVec = { camera._viewMatrix.m[1][0], camera._viewMatrix.m[1][1], camera._viewMatrix.m[1][2] };

	camRightVec.Normalize();
	camLookVec.Normalize();
	camUpVec.Normalize();

	outFrustum._nearFace = { camLookVec , camera._cameraPosition + camLookVec * camera._cameraNear};
	outFrustum._farFace = { -camLookVec , camera._cameraPosition + camLookVec * camera._cameraFar};

	DUOLMath::Vector3 FarVec = camLookVec * camera._cameraFar;

	float halfVertical = camera._cameraFar * tanf(camera._cameraVerticalFOV * 0.5f);
	float halfHorizon = halfVertical * camera._aspectRatio;

	constexpr  DUOLMath::Vector3 right{1.f, 0.f, 0.f};
	constexpr  DUOLMath::Vector3 up{0.f, 1.f, 0.f};

	DUOLMath::Vector3 rightNormal = FarVec + camRightVec * halfHorizon;
	DUOLMath::Vector3 lefttNormal = FarVec - camRightVec * halfHorizon;

	outFrustum._rightFace = { rightNormal.Cross(up) , camera._cameraPosition};
	outFrustum._leftFace = { up.Cross(lefttNormal) , camera._cameraPosition};

	DUOLMath::Vector3 upNormal =   FarVec + camUpVec * halfVertical;
	DUOLMath::Vector3 downNormal = FarVec - camUpVec * halfVertical;

	outFrustum._topFace = { upNormal.Cross(right) , camera._cameraPosition };
	outFrustum._leftFace = { right.Cross(downNormal) , camera._cameraPosition };\

	outFrustum._rightFace._normal.Normalize();
	outFrustum._leftFace._normal.Normalize();
	outFrustum._topFace._normal.Normalize();
	outFrustum._bottomFace._normal.Normalize();
}
