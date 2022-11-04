#pragma once
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
	ID3D11Texture2D* m_RenderTargetTexture; // 여기에 버퍼를 저장해 놓을 것.. 
	ID3D11RenderTargetView* m_RenderTargetView; // 얘는 배열로 관리해야 함. DX에서 OMSetRendertargets을 배열로 넘겨줌.
	ID3D11ShaderResourceView* m_ShaderResourceView; // 이 친구로 쉐이더에 넘겨줌.
};

