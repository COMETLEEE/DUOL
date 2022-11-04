#pragma once
class Camera
{
private:
	XMMATRIX m_CameraView;
	XMMATRIX m_CameraProj;

	XMMATRIX m_CurrentViewProj; // ��� ���� ����! ������ ����������
	XMMATRIX m_PrevViewProj; // ��� ���� ����! ������ ����������

	// �׸��ڵ� �ϰ� ����������??
	XMMATRIX m_ShadowView;
	XMMATRIX m_ShadowProj;
public:
	void CameraUpdate(const XMMATRIX& _View, const XMMATRIX& _Proj, const XMFLOAT3& _Pos);
	void ShadowUpdate(const XMMATRIX& _View, const XMMATRIX& _Proj);



	XMMATRIX& GetCurrentViewProj() { return m_CurrentViewProj; }
	XMMATRIX& GetPrevViewProj() { return m_PrevViewProj; }

	XMMATRIX GetCameraView() { return m_CameraView; }
	XMMATRIX GetCameraProj() { return m_CameraProj; }

	XMMATRIX GetShadowView() { return m_ShadowView; }
	XMMATRIX GetShadowProj() { return m_ShadowProj; }
};

