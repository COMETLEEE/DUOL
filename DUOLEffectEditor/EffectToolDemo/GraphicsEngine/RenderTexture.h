#pragma once
namespace MuscleGrapics
{
	class RenderTexture
	{
	public:
		RenderTexture();

		~RenderTexture();

		bool Initialize(int _Width, int _Height);

		void Release();

		void ClearRenderTarget();

		ID3D11RenderTargetView* GetRenderTargetView();

		ID3D11ShaderResourceView* GetSRV();

	private:
		ID3D11Texture2D* _renderTargetTexture; // ���⿡ ���۸� ������ ���� ��.. 

		ID3D11RenderTargetView* _renderTargetView; // ��� �迭�� �����ؾ� ��. DX���� OMSetRendertargets�� �迭�� �Ѱ���.

		ID3D11ShaderResourceView* _shaderResourceView; // �� ģ���� ���̴��� �Ѱ���.

	};
}
