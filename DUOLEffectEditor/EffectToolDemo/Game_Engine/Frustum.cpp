#include "pch.h"
#include "Frustum.h"

namespace Muscle
{
	Frustum::Frustum(std::shared_ptr<GameObject> _GameObj)
	{
		//m_Camera = _GameObj->GetComponent<Camera>();
		//m_GameObject = _GameObj;
		//float _NearZ = m_Camera->GetNearZ(); // 가까운 절두체 평면 거리 
		//float _FarZ = m_Camera->GetFarZ(); // 먼 절두체 평면 거리
		//float _NearWindowHeight = m_Camera->GetNearWindowHeight() / 2; // 가까운 화면 높이 half
		//float _FarWindowHeight = m_Camera->GetFarWindowHeight() / 2; // 먼 화면 높이 half

		//float _FarWindowWidth = m_Camera->GetFarWindowWidth() / 2; // 먼 화면 높이
		//float _NearWindowWidth = m_Camera->GetNearWindowWidth() / 2; // 가까운 화면 높이


		//XMFLOAT3 _Box[] =
		//{
		//	m_VertexPos[0] = XMFLOAT3(_FarWindowWidth, _FarWindowHeight, _FarZ),
		//	m_VertexPos[1] = XMFLOAT3(-_FarWindowWidth, _FarWindowHeight, _FarZ),
		//	m_VertexPos[2] = XMFLOAT3(-_NearWindowWidth, _NearWindowHeight, _NearZ),
		//	m_VertexPos[3] = XMFLOAT3(_NearWindowWidth, _NearWindowHeight, _NearZ),
		//	m_VertexPos[4] = XMFLOAT3(_FarWindowWidth, -_FarWindowHeight, _FarZ),
		//	m_VertexPos[5] = XMFLOAT3(-_FarWindowWidth, -_FarWindowHeight, _FarZ),
		//	m_VertexPos[6] = XMFLOAT3(-_NearWindowWidth, -_NearWindowHeight, _NearZ),
		//	m_VertexPos[7] = XMFLOAT3(_NearWindowWidth, -_NearWindowHeight, _NearZ)
		//};

		//m_Plane[0] = new Plane(m_VertexPos[7], m_VertexPos[3], m_VertexPos[0]);
		//m_Plane[1] = new Plane(m_VertexPos[3], m_VertexPos[2], m_VertexPos[0]);
		//m_Plane[2] = new Plane(m_VertexPos[6], m_VertexPos[3], m_VertexPos[7]);

		//m_Plane[3] = new Plane(m_VertexPos[5], m_VertexPos[1], m_VertexPos[6]);
		//m_Plane[4] = new Plane(m_VertexPos[6], m_VertexPos[7], m_VertexPos[4]);
		//m_Plane[5] = new Plane(m_VertexPos[0], m_VertexPos[5], m_VertexPos[4]);

		//GizmoInfo _Info =
		//{
		//	Box,
		//	0,
		//	_Box
		//};
		//GizmoRenderer* _Gizmo = _GameObj->AddComponent<GizmoRenderer>();
		//_Gizmo->Initialize(&_Info);
		//_Gizmo->Start();
	}

	Frustum::~Frustum()
	{
		//for (unsigned int i = 0; i < 6; i++)
		//{
		//	if (m_Plane[i])
		//		delete m_Plane[i];
		//}
	}

	bool Frustum::CheckDot(Vector3& _WorldVertexPos)
	{
		//for (int j = 0; j < 6; j++)
		//{
		//	// 6개의 평면에 대입.
		//	XMVECTOR _P1 = _WorldVertexPos - m_WorldPoint[j];

		//	XMVECTOR _P2 = XMVector3Dot(_P1, m_WorldNomal[j]);
		//	float _Dot = XMVector3Dot(m_WorldNomal[j], _P1).m128_f32[0];
		//	// 모두 음수면 프러스텀 안에 있다는 뜻.
		//	if (_Dot > 0)
		//		return false;
		//}

		return true;
	}

	void Frustum::OnResize()
	{
		//for (unsigned int i = 0; i < 6; i++)
		//{
		//	delete m_Plane[i];
		//}

		//float _NearZ = m_Camera->GetNearZ(); // 가까운 절두체 평면 거리 
		//float _FarZ = m_Camera->GetFarZ(); // 먼 절두체 평면 거리
		//float _NearWindowHeight = m_Camera->GetNearWindowHeight() / 2; // 가까운 화면 높이 half
		//float _FarWindowHeight = m_Camera->GetFarWindowHeight() / 2; // 먼 화면 높이 half

		//float _FarWindowWidth = m_Camera->GetFarWindowWidth() / 2; // 먼 화면 높이
		//float _NearWindowWidth = m_Camera->GetNearWindowWidth() / 2; // 가까운 화면 높이


		//XMFLOAT3 _Box[] =
		//{
		//	m_VertexPos[0] = XMFLOAT3(_FarWindowWidth, _FarWindowHeight, _FarZ),
		//	m_VertexPos[1] = XMFLOAT3(-_FarWindowWidth, _FarWindowHeight, _FarZ),
		//	m_VertexPos[2] = XMFLOAT3(-_NearWindowWidth, _NearWindowHeight, _NearZ),
		//	m_VertexPos[3] = XMFLOAT3(_NearWindowWidth, _NearWindowHeight, _NearZ),
		//	m_VertexPos[4] = XMFLOAT3(_FarWindowWidth, -_FarWindowHeight, _FarZ),
		//	m_VertexPos[5] = XMFLOAT3(-_FarWindowWidth, -_FarWindowHeight, _FarZ),
		//	m_VertexPos[6] = XMFLOAT3(-_NearWindowWidth, -_NearWindowHeight, _NearZ),
		//	m_VertexPos[7] = XMFLOAT3(_NearWindowWidth, -_NearWindowHeight, _NearZ)
		//};

		//m_Plane[0] = new Plane(m_VertexPos[7], m_VertexPos[3], m_VertexPos[0]);
		//m_Plane[1] = new Plane(m_VertexPos[3], m_VertexPos[2], m_VertexPos[0]);
		//m_Plane[2] = new Plane(m_VertexPos[6], m_VertexPos[3], m_VertexPos[7]);

		//m_Plane[3] = new Plane(m_VertexPos[5], m_VertexPos[1], m_VertexPos[6]);
		//m_Plane[4] = new Plane(m_VertexPos[6], m_VertexPos[7], m_VertexPos[4]);
		//m_Plane[5] = new Plane(m_VertexPos[0], m_VertexPos[5], m_VertexPos[4]);

		//GizmoInfo _Info =
		//{
		//	Box,
		//	0,
		//	_Box
		//};
		//GizmoRenderer* _Gizmo = m_GameObject->GetComponent<GizmoRenderer>();
		//_Gizmo->Initialize(&_Info);
		//_Gizmo->Start();
	}

	void Frustum::Update()
	{
		//ObjectManager* _ObjManager = MuscleEngine::GetInstance()->GetObjManager();


		//XMMATRIX _WorldTM = m_GameObject->GetTransform()->GetXMWorldTM();
		//XMMATRIX WorldInversTranspose = MathHelper::InverseTranspose(_WorldTM); // 노말이니깐 역전치?

		//for (int i = 0; i < 6; i++)
		//{

		//	m_WorldNomal[i] = XMVector3Transform(m_Plane[i]->m_Normal, WorldInversTranspose);
		//	m_WorldPoint[i] = XMVector3Transform(m_Plane[i]->m_Point, _WorldTM);
		//}

		//for (auto& iter : _ObjManager->m_vectorObjects)
		//{
		//	bool _isRender = false;
		//	if (iter->m_BoxVolume)
		//	{
		//		// 컬링 시작
		//		// 
		//		// 바운딩 박스의 버텍스
		//		XMMATRIX _BoxWorldTM = iter->GetTransform()->GetXMWorldTM();
		//		for (int i = 0; i < 8; i++)
		//		{
		//			// 월드 좌표로 변경.
		//			XMVECTOR _WorldVertexPos = XMVector3Transform(XMLoadFloat3(&iter->m_BoxVolume->GetVertexPos()[i]), _BoxWorldTM);

		//			if (CheckDot(_WorldVertexPos))
		//			{
		//				// 화면에 보여주기.
		//				_isRender = true;
		//				break;
		//			}
		//		}


		//		iter->SetIsRender(_isRender);

		//	}
		//}

	}
}