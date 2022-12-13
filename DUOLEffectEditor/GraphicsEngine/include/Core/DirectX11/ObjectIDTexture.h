#pragma once

#include "RenderTexture.h"

namespace MuscleGrapics
{
	class ObjectIDTexture : public RenderTexture
	{
	public:
		ObjectIDTexture() = default;

		virtual ~ObjectIDTexture() ;

	public:
		virtual bool Initialize(int width, int height) override;

		virtual void Release() override;

		ID3D11Texture2D* GetCopyTargetTexture();
	private:
		ID3D11Texture2D* _copyTexture;
	};
}