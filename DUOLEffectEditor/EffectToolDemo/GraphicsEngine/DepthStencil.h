#pragma once
class DepthStencil
{
public:
	DepthStencil();
	~DepthStencil();
private:
	Depth* m_DepthStencil[2];
	ID3D11DepthStencilState* m_OffDepthStencilState; // ���� Off�� ���� ���ٽ� ��
public:
	void OnDepthStencil(int _Num); // 0 �Ϲ� 1 ������ ����
	ID3D11DepthStencilView* GetDpethStencilView(int _Num); // 0 �Ϲ� 1 ������ ����
	void OffDepthStencil();
	void Clear();
	void OnResize();
	void Finalize();
};

