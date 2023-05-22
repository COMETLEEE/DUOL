#pragma once
#include <d3d11.h>

#include "Core/Resource/Resource/ResourceBase.h"

class ID3D11RenderTargetView;

class ID3D11ShaderResourceView;

class ID3D11Texture2D;

namespace MuscleGrapics
{
	class RenderTexture : public ResourceBase
	{
	public:
		RenderTexture();

		virtual ~RenderTexture();

		virtual bool Initialize(int _Width, int _Height); // Default

		bool CreateTexture2D(D3D11_TEXTURE2D_DESC* desc);

		bool CreateSRV(D3D11_SHADER_RESOURCE_VIEW_DESC* desc);

		bool CreateRTV(D3D11_RENDER_TARGET_VIEW_DESC* desc);

		virtual void Release();

		void ClearRenderTarget();

		void ClearRenderTarget(float clearColor[4]);

		ID3D11RenderTargetView* GetRTV();

		ID3D11ShaderResourceView* GetSRV();

		ID3D11Texture2D* GetRenderTargetTexture();
	private:
		float _scale;

		ID3D11Device* _device; // 캐싱
	public:
		void SetScale(float scale);

	protected:
		ID3D11Texture2D* _renderTargetTexture; // 여기에 버퍼를 저장해 놓을 것.. 

		ID3D11RenderTargetView* _renderTargetView; // 얘는 배열로 관리해야 함. DX에서 OMSetRendertargets을 배열로 넘겨줌.

		ID3D11ShaderResourceView* _shaderResourceView; // 이 친구로 쉐이더에 넘겨줌.

	};
}
