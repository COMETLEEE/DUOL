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

	protected:
		ID3D11Texture2D* _renderTargetTexture; // ���⿡ ���۸� ������ ���� ��.. 

		ID3D11RenderTargetView* _renderTargetView; // ��� �迭�� �����ؾ� ��. DX���� OMSetRendertargets�� �迭�� �Ѱ���.

		ID3D11ShaderResourceView* _shaderResourceView; // �� ģ���� ���̴��� �Ѱ���.

	};
}
