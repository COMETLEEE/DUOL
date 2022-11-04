#pragma once
class Depth
{
public:
	Depth();
	~Depth();
private:
	// ���� ���۴� �̹��� �ڷḦ ���� �ʴ� �ؽ�ó�� ��.
	// ���� ���۴� �� �ȼ��� ���� ������ ��´�.
	ID3D11Texture2D* m_DepthStencilBuffer; // ���� ���ٽ� ���� , �����ϴ� ��.
	ID3D11DepthStencilView* m_DepthStencilView; // ���� ���ٽ� �� , �׸��� ��.
	ID3D11DepthStencilState* m_DepthStencilState; // ���� ���ٽ� ������Ʈ // ��� �׸���.
public:
	void Clear();
	void OnResize();
	void Finalize();
	void OMsetDepth();
	ID3D11DepthStencilView* GetDeptStencilView();
};

