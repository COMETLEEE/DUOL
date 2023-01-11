#pragma once
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderConstantBuffer.h"

namespace DUOLGraphicsEngine
{
	class ShadowHelper
	{
		void CalculateViewFrustumPoint(ConstantBufferPerFrame& perFrameBuffer, float near, float far, float range);

		void CalculateViewFrustumPoint(ConstantBufferPerFrame& perFrameBuffer, float near, float far, float FovAngleY, float AspectRatio);

	};
}