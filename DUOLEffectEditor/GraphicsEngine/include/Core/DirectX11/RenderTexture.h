#pragma once
class ID3D11RenderTargetView;

class ID3D11ShaderResourceView;

class ID3D11Texture2D;

namespace MuscleGrapics
{
	class RenderTexture
	{
	public:
		RenderTexture();

		virtual ~RenderTexture();

		virtual bool Initialize(int _Width, int _Height);

		virtual void Release();

		void ClearRenderTarget();

		ID3D11RenderTargetView* GetRenderTargetView();

		ID3D11ShaderResourceView* GetSRV();

		ID3D11Texture2D* GetRenderTargetTexture();
	private:
		float _scale;

	public:
		void SetScale(float scale);

	protected:
		ID3D11Texture2D* _renderTargetTexture; // 여기에 버퍼를 저장해 놓을 것.. 

		ID3D11RenderTargetView* _renderTargetView; // 얘는 배열로 관리해야 함. DX에서 OMSetRendertargets을 배열로 넘겨줌.

		ID3D11ShaderResourceView* _shaderResourceView; // 이 친구로 쉐이더에 넘겨줌.

	};
}
