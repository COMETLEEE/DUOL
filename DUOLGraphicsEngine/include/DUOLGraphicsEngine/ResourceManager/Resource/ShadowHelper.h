#pragma once
#include "DUOLMath/DUOLMath.h"

namespace DUOLGraphicsEngine
{
	struct ConstantBufferPerCamera;

	struct CascadeShadowSlice
	{
		DUOLMath::Vector3 _frustumCenter;

		float _frustumRadius = 0;

		DUOLMath::Vector3 _orthoMin;

		DUOLMath::Vector3 _orthoMax;
	};

	struct CascadeShadowInfo
	{
		DUOLMath::Matrix shadowMatrix[4];

		float _cascadeSliceOffset[4];

		float _cascadeScale[4];
	};

	class ShadowHelper
	{
	public:
		//void CalculateCascadeShadowSlices(ConstantBufferPerFrame& perFrameBuffer, float near, float far, float range);

		//일단은 무조건 네개만 넣으세요..
		static void CalculateCascadeShadowSlices(ConstantBufferPerCamera& perCameraBuffer, float near, float far, float FovAngleY, float AspectRatio, float* cascadeOffset, CascadeShadowSlice cascadeShadowInfos[4], int
		                                         shadowMapTextureSize);

		static void CalculateCascadeShadowSlices2(ConstantBufferPerCamera& perCameraBuffer, DUOLMath::Matrix, float near, float far, float FovAngleY, float AspectRatio, float* cascadeOffset, CascadeShadowSlice cascadeShadowInfos[4], int
		                                          shadowMapTextureSize, DUOLMath::Vector3 lightDirection);

		static void CalcuateViewProjectionMatrixFromCascadeSlice(const CascadeShadowSlice& shadowInfo, const DUOLMath::Vector3& lightDir, DUOLMath::Matrix& outMatrix);

		static void CalcuateViewProjectionMatrixFromCascadeSlice2(const CascadeShadowSlice& shadowInfo, const DUOLMath::Vector3& lightDir, DUOLMath::Matrix& outMatrix);
	};
}