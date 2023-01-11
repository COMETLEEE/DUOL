#pragma once
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderConstantBuffer.h"

namespace DUOLGraphicsEngine
{
	struct CascadeShadowInfo
	{
		DUOLMath::Vector3 _frustumCenter;

		float _frustumRadius;
	};


	class ShadowHelper
	{
		void CalculateCascadeShadowSlices(ConstantBufferPerFrame& perFrameBuffer, float near, float far, float range);

		//�ϴ��� ������ �װ��� ��������..
		void CalculateCascadeShadowSlices(ConstantBufferPerFrame& perFrameBuffer, float near, float far, float FovAngleY, float AspectRatio, CascadeShadowInfo cascadeShadowInfos[4]);

		void CalcuateViewProjectionMatrixFromCascadeSlice(const CascadeShadowInfo& shadowInfo, const DUOLMath::Vector3& lightDir, DUOLMath::Matrix& outMatrix);
	};
}