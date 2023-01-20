#include "DUOLGraphicsEngine/ResourceManager/Resource/ShadowHelper.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderConstantBuffer.h"
#include <cmath>

namespace DUOLGraphicsEngine
{
	void DUOLGraphicsEngine::ShadowHelper::CalculateCascadeShadowSlices(ConstantBufferPerFrame& perFrameBuffer, float near, float far, float fovAngleY, float aspectRatio, float* cascadeOffset, CascadeShadowSlice cascadeShadowInfos[4])
	{
		constexpr int cascadeCnt = 4;

		//zero to 1.f
		float cascadeSplits[cascadeCnt + 1] = {0.f,};
		for(int idx = 1; idx < cascadeCnt + 1; idx++)
		{
			cascadeSplits[idx] = cascadeOffset[idx-1];
		}

		DUOLMath::Vector3 frustumCornerPoint[8] =
		{
			 {-1.f, +1.f, +0.f} //near
			,{-1.f, -1.f, +0.f} //near
			,{+1.f, -1.f, +0.f} //near
			,{+1.f, +1.f, +0.f} //near
			,{-1.f, +1.f, +1.f} //far
			,{-1.f, -1.f, +1.f}	//far
			,{+1.f, -1.f, +1.f} //far
			,{+1.f, +1.f, +1.f} //far
		};

		auto viewProjInv = perFrameBuffer._camera._viewProjectionInverseTransposeMatrix.Transpose();

		for (int pointIdx = 0; pointIdx < 8; ++pointIdx)
		{
			frustumCornerPoint[pointIdx] = DUOLMath::Vector3::Transform(frustumCornerPoint[pointIdx], viewProjInv);
		}

		DUOLMath::Vector3 cornerVector[4];

		for (int cornerIdx = 0; cornerIdx < 4; ++cornerIdx)
		{
			cornerVector[cornerIdx] = frustumCornerPoint[cornerIdx + 4] - frustumCornerPoint[cornerIdx];
		}

		//프러스텀을 자른다
		DUOLMath::Vector3 cascadeBoundingBox[cascadeCnt][8];

		for (int cascadeIdx = 0; cascadeIdx < cascadeCnt; ++cascadeIdx)
		{
			for (int boundingBoxIdx = 0; boundingBoxIdx < 4; ++boundingBoxIdx)
			{
				DUOLMath::Vector3 nearDist = cornerVector[boundingBoxIdx] * cascadeSplits[cascadeIdx];
				DUOLMath::Vector3 farDist = cornerVector[boundingBoxIdx] * cascadeSplits[cascadeIdx + 1];

				cascadeBoundingBox[cascadeIdx][boundingBoxIdx] = frustumCornerPoint[boundingBoxIdx] + nearDist;
				cascadeBoundingBox[cascadeIdx][boundingBoxIdx + 4] = frustumCornerPoint[boundingBoxIdx] + farDist;
			}
		}

		//중점구하기
		for (int cascadeIdx = 0; cascadeIdx < cascadeCnt; cascadeIdx++)
		{
			for (int boundingBoxPointIdx = 0; boundingBoxPointIdx < 8; ++boundingBoxPointIdx)
			{
				cascadeShadowInfos[cascadeIdx]._frustumCenter += cascadeBoundingBox[cascadeIdx][boundingBoxPointIdx];
			}

			cascadeShadowInfos[cascadeIdx]._frustumCenter /= 8.f;
		}


		for (int cascadeIdx = 0; cascadeIdx < cascadeCnt; cascadeIdx++) 
		{
			for (int boundingBoxPointIdx = 0; boundingBoxPointIdx < 8; boundingBoxPointIdx++)
			{
				float dist = (cascadeBoundingBox[cascadeIdx][boundingBoxPointIdx] - cascadeShadowInfos[cascadeIdx]._frustumCenter).Length();
				cascadeShadowInfos[cascadeIdx]._frustumRadius = std::max<float>(cascadeShadowInfos[cascadeIdx]._frustumRadius,dist);
			}

			//오차줄이기용 코드인것같다.
			cascadeShadowInfos[cascadeIdx]._frustumRadius = std::ceil(cascadeShadowInfos[cascadeIdx]._frustumRadius * 16.f) / 16.f;
		}
	}

	void ShadowHelper::CalcuateViewProjectionMatrixFromCascadeSlice(const CascadeShadowSlice& shadowInfo, const DUOLMath::Vector3& lightDir, DUOLMath::Matrix& outMatrix)
	{
		DUOLMath::Vector3 light = lightDir;
		light.Normalize();

		//쉐도우 카메라의 포지션과 proj matrix near, far 하드코딩되어있음.
		DUOLMath::Vector3 shadowCamPos = shadowInfo._frustumCenter - (light * 250);

		auto view = DUOLMath::Matrix::CreateLookAt(shadowCamPos, shadowInfo._frustumCenter, DUOLMath::Vector3(0.f, 1.f, 0.f));
   		auto proj = DUOLMath::Matrix::CreateOrthographicOffCenter(-shadowInfo._frustumRadius, shadowInfo._frustumRadius, -shadowInfo._frustumRadius, shadowInfo._frustumRadius, 0 , 1000);

		outMatrix = view * proj;
	}
}
