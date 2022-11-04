#include "pch.h"
#include "Camera.h"

void Camera::CameraUpdate(const XMMATRIX& _View, const XMMATRIX& _Proj, const XMFLOAT3& _Pos)
{
	m_PrevViewProj = m_CameraView * m_CameraProj;
	m_CameraView = _View;
	m_CameraProj = _Proj;
	m_CurrentViewProj = m_CameraView * m_CameraProj;
	Effects::LightFX->SetEyePosW(_Pos);
}

void Camera::ShadowUpdate(const XMMATRIX& _View, const XMMATRIX& _Proj)
{
	m_ShadowView = _View;
	m_ShadowProj = _Proj;
}
