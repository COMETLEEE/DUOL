#pragma once
class Camera
{
private:
	XMMATRIX m_CameraView;
	XMMATRIX m_CameraProj;

	XMMATRIX m_CurrentViewProj; // 모션 블러를 위함! 현재의 뷰프로젝션
	XMMATRIX m_PrevViewProj; // 모션 블러를 위함! 이전의 뷰프로젝션

	// 그림자도 니가 갖고있을래??
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

