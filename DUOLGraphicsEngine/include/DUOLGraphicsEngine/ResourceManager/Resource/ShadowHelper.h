#pragma once
#include "DUOLMath/DUOLMath.h"

namespace DUOLGraphicsEngine
{
	struct ConstantBufferPerCamera;

	struct CascadeShadowSlice
	{
		DUOLMath::Vector3 _frustumCenter;

		float _frustumRadius = 0;
	};

	struct CascadeShadowInfo
	{
		DUOLMath::Matrix shadowMatrix[4];

		float _cascadeSliceOffset[4];
	};

	class ShadowHelper
	{
	public:
		//void CalculateCascadeShadowSlices(ConstantBufferPerFrame& perFrameBuffer, float near, float far, float range);

		//일단은 무조건 네개만 넣으세요..
		static void CalculateCascadeShadowSlices(ConstantBufferPerCamera& perCameraBuffer, float near, float far, float FovAngleY, float AspectRatio, float* cascadeOffset, CascadeShadowSlice cascadeShadowInfos[4]);

		static void CalcuateViewProjectionMatrixFromCascadeSlice(const CascadeShadowSlice& shadowInfo, const DUOLMath::Vector3& lightDir, DUOLMath::Matrix& outMatrix);
	};
}