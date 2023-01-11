#include "ShadowHelper.h"

namespace DUOLGraphicsEngine
{
	void DUOLGraphicsEngine::ShadowHelper::CalculateViewFrustumPoint(ConstantBufferPerFrame& perFrameBuffer, float near, float far, float fovAngleY, float aspectRatio)
	{
		float sinFovY = sin(0.5f * fovAngleY);
		float cosFovY = cos(0.5f * fovAngleY);

		float height = cosFovY / sinFovY; // 1 / tan(a/2)
		float width = height / aspectRatio; // 1 / (tan(a/2) * ratio) 

	}
}
