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
	ID3D11Texture2D* m_RenderTargetTexture; // ���⿡ ���۸� ������ ���� ��.. 
	ID3D11RenderTargetView* m_RenderTargetView; // ��� �迭�� �����ؾ� ��. DX���� OMSetRendertargets�� �迭�� �Ѱ���.
	ID3D11ShaderResourceView* m_ShaderResourceView; // �� ģ���� ���̴��� �Ѱ���.
};

