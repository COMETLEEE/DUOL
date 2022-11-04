#include "pch.h"
#include "Camera.h"
#include "Frustum.h"


namespace Muscle
{

	Camera::Camera(std::shared_ptr<GameObject> _GameObj) : IComponents(_GameObj),
		m_NearZ(0), m_FarZ(0), m_Aspect(0), m_FovY(0), m_NearWindowHeight(0), m_FarWindowHeight(0), m_Proj()
		, m_isCulling(true)
	{

		m_Transform = m_GameObject.lock()->GetTransform();
		m_isMainCamera = false;
		_cameraInfo = std::make_shared<CameraInfo>();
	}

	Camera::~Camera()
	{
		delete m_Frustum;
	}

	Matrix Camera::View()
	{
		return XMLoadFloat4x4(&m_View);
	}

	Matrix Camera::Proj()
	{
		return XMLoadFloat4x4(&m_Proj);
	}


	std::shared_ptr<GameObject> Camera::Pick(int _x, int _y)
	{

		//XMMATRIX P = Proj();

		//// ���� ��
		//float vx = (+2.0f * _x / MuscleEngine::GetInstance()->GetGraphicsEngine()->GetWidth() - 1.0f) / P.r[0].m128_f32[0];
		//float vy = (-2.0f * _y / MuscleEngine::GetInstance()->GetGraphicsEngine()->GetHeight() + 1.0f) / P.r[1].m128_f32[1];

		//// Ray definition in view space.
		//XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		//XMVECTOR rayDir = XMVectorSet(vx, vy, 1.0f, 0.0f);

		//// �� ��
		//XMMATRIX V = View();
		//XMMATRIX invView = XMMatrixInverse(nullptr, V);

		//XMMATRIX W = m_Transform->GetXMWorldTM();
		//XMMATRIX invWorld = XMMatrixInverse(nullptr, W);
		////rayOrigin = XMVector3Transform(rayOrigin, invView);
		//rayOrigin = XMVector3TransformCoord(rayOrigin, invView);

		//rayDir = XMVector3Transform(rayDir, invView);
		//rayDir = rayDir - rayOrigin;
		//rayDir = XMVector3Normalize(rayDir);


		////����� ���� ��.
		//XMFLOAT3 _Start;
		//XMFLOAT3 _End;
		//XMStoreFloat3(&_Start, rayOrigin);
		//XMStoreFloat3(&_End, rayOrigin + (rayDir * 1000));

		//////Renderer 
		//XMFLOAT3 _Info[2];
		//_Info[0] = _Start;
		//_Info[1] = _End;

		//GameObject* _GameObject = new GameObject();
		//GizmoInfo _info =
		//{
		//Line,
		//0,
		//nullptr,
		//_Info
		//};
		//_GameObject->AddComponent<GizmoRenderer>()->Initialize(&_info);
		//MuscleEngine::GetInstance()->GetObjManager()->InsertObject(_GameObject);


		//GameObject* picked = nullptr;
		//float saveNear = 1000000.f;

		//auto& _Objects = MuscleEngine::GetInstance()->GetObjManager()->m_vectorObjects;
		//for (auto& _GameObject : _Objects)
		//{
		//	if (!_GameObject->m_BoxVolume)
		//		continue;

		//	// �ڽ� ������ ���ؽ��� �޾ƿ�.
		//	// �ڽ� ���� ������ ��ŷ �ȵ�!
		//	XMFLOAT3* vertices = _GameObject->m_BoxVolume->GetVertexPos();

		//	XMVECTOR minVert = { vertices[6].x, vertices[6].y, vertices[6].z };
		//	XMVECTOR maxVert = { vertices[0].x, vertices[0].y, vertices[0].z };

		//	XMMATRIX worldTM = _GameObject->GetTransform()->GetXMWorldTM();
		//	minVert = XMVector3Transform(minVert, worldTM);
		//	maxVert = XMVector3Transform(maxVert, worldTM);

		//	// https://mycom333.blogspot.com/2011/12/slab-kay-slab-aabb-ray-r-t-o-t-d-o-t-d.html
		//	// t: �������� �������� �����ϴµ� ���Ǵ� ����
		//	// ��� ���� �� �� x�� ���� n*x + D = 0 �� �����ϴµ� x�� t���� ������ ������ �������� ��ġ�Ѵ�.
		//	// n * (r(t)) + D = 0 �̷���..  Ray(����) r(t) = o + td [ o : ����(���⼱ ���� ī�޶����?), d : ���� ����]
		//	// t = (-(n*o) - D) / ( n*d ) �� �ǰ� AABB Ư¡�� ǥ�� ������ ���� ���Ƽ� nx = (1, 0, 0) �̱⶧���� 
		//	// nx * o = (1, 0, 0) * (ox, oy, oz) = ox �̷��� �ȴ�.
		//	// �� t = (-o - D) / d �ΰ��̴�. 
		//	// 
		//	// �� -o -D / d�ε� 
		//	// t = D - o / d�� �ٲ�°ž�??
		//	float txMin = (minVert.m128_f32[0] - rayOrigin.m128_f32[0]) / rayDir.m128_f32[0];	    // (min.x - rayOrigin.x) / rayDir.x
		//	float txMax = (maxVert.m128_f32[0] - rayOrigin.m128_f32[0]) / rayDir.m128_f32[0]; 	// (max.x - rayOrigin.x) / rayDir.x
		//	float tyMin = (minVert.m128_f32[1] - rayOrigin.m128_f32[1]) / rayDir.m128_f32[1];			// (min.y - rayOrigin.y) / rayDir.y
		//	float tyMax = (maxVert.m128_f32[1] - rayOrigin.m128_f32[1]) / rayDir.m128_f32[1]; 		// (max.y - rayOrigin.y) / rayDir.y
		//	float tzMin = (minVert.m128_f32[2] - rayOrigin.m128_f32[2]) / rayDir.m128_f32[2];	    // (min.z - rayOrigin.z) / rayDir.z
		//	float tzMax = (maxVert.m128_f32[2] - rayOrigin.m128_f32[2]) / rayDir.m128_f32[2]; 	// (max.z - rayOrigin.z) / rayDir.z

		//	// https://gist.github.com/DomNomNom/46bb1ce47f68d255fd5d
		//	// �� ���� ����.. min, max �����ش�..?
		//	XMFLOAT3 tMin = { min(txMin, txMax), min(tyMin, tyMax), min(tzMin, tzMax) };
		//	XMFLOAT3 tMax = { max(txMin, txMax), max(tyMin, tyMax), max(tzMin, tzMax) };

		//	float tNear = max(max(tMin.x, tMin.y), tMin.z);			// min�� �ִ밪
		//	float tFar = min(min(tMax.x, tMax.y), tMax.z);			// max�� �ּҰ�


		//	// https://dlemrcnd.tistory.com/91
		//	// ����� ������ Ŀ���� �浹X 
		//	// min max min max �浹 �� �Ҷ�.
		//	// �浹�� �� min min max max
		//	if (tNear > tFar)
		//		continue;

		//	// �������� ���� ������ ����������..
		//	if (saveNear > tNear)
		//	{
		//		saveNear = tNear;
		//		picked = _GameObject;
		//	}
		//}
		//return picked;
		return nullptr;
	}

	void Camera::Start()
	{
		m_Frustum = new Frustum(m_GameObject.lock());
		SetLens();
		Update();
	}

	void Camera::Update()
	{

	}

	void Camera::LateUpdate()
	{

		_cameraInfo->_prevProjMatrix = Proj();
		_cameraInfo->_prevViewMatrix = View();

		Vector3 R = m_Transform->GetXMRight();
		Vector3 U = m_Transform->GetXMUp();
		Vector3 L = m_Transform->GetXMLook();
		Vector3 P = m_Transform->GetPosition();

		//// Keep camera's axes orthogonal to each other and of unit length.
		//L = XMVector3Normalize(L);
		//U = XMVector3Normalize(XMVector3Cross(L, R));

		//// U, L already ortho-normal, so no need to normalize cross product.
		//R = XMVector3Cross(U, L);

		// Fill in the view matrix entries.
		float x = -XMVector3Dot(P, R).m128_f32[0];
		float y = -XMVector3Dot(P, U).m128_f32[0];
		float z = -XMVector3Dot(P, L).m128_f32[0];
		// (Tansform-1) * (Roatate-1) �� ����ȭ �� ��.. p.225

		Vector3 m_Right = m_Transform->GetRight();
		Vector3 m_Up = m_Transform->GetUp();
		Vector3 m_Look = m_Transform->GetLook();

		XMStoreFloat3(&m_Right, R);
		XMStoreFloat3(&m_Up, U);
		XMStoreFloat3(&m_Look, L);

		m_View(0, 0) = m_Right.x;
		m_View(1, 0) = m_Right.y;
		m_View(2, 0) = m_Right.z;
		m_View(3, 0) = x;

		m_View(0, 1) = m_Up.x;
		m_View(1, 1) = m_Up.y;
		m_View(2, 1) = m_Up.z;
		m_View(3, 1) = y;

		m_View(0, 2) = m_Look.x;
		m_View(1, 2) = m_Look.y;
		m_View(2, 2) = m_Look.z;
		m_View(3, 2) = z;

		m_View(0, 3) = 0.0f;
		m_View(1, 3) = 0.0f;
		m_View(2, 3) = 0.0f;
		m_View(3, 3) = 1.0f;
		//View ����̶�! ī�޶��� ������̴�! �������� ������!

		if (m_isMainCamera)
		{
			//MuscleEngine::GetInstance()->GetGraphicsEngine()->CameraUpdate(View(), Proj(), m_Transform->GetPosition());
			if (m_isCulling)
				m_Frustum->Update();
		}

		_cameraInfo->_cameraWorldPosition = m_Transform->GetPosition();
		_cameraInfo->_projMatrix = Proj();
		_cameraInfo->_viewMatrix = View();

	}

	void Camera::Render()
	{
	}


	float Camera::GetNearZ() const
	{
		return m_NearZ;
	}

	float Camera::GetFarZ() const
	{
		return m_FarZ;
	}

	float Camera::GetAspect() const
	{
		return m_Aspect;
	}

	float Camera::GetFovY() const
	{
		return m_FovY;
	}

	float Camera::GetFovX() const
	{
		float halfWidth = 0.5f * GetNearWindowWidth();
		return 2.0f * atan(halfWidth / m_NearZ);
	}

	float Camera::GetNearWindowWidth() const
	{
		return m_NearWindowHeight * m_Aspect;
	}

	float Camera::GetNearWindowHeight() const
	{
		return m_NearWindowHeight;
	}

	float Camera::GetFarWindowWidth() const
	{
		return m_FarWindowHeight * m_Aspect;
	}

	float Camera::GetFarWindowHeight() const
	{
		return m_FarWindowHeight;
	}

	void Camera::SetLens()
	{

		RECT _rect;
		GetClientRect(MuscleEngine::GetInstance()->GetHWND(), &_rect);
		m_FovY = 0.25f * Muscle::PI;
		m_Aspect = (float)_rect.right / (float)_rect.bottom;
		m_NearZ = 1.0f;
		m_FarZ = 300.f;

		m_NearWindowHeight = 2.0f * m_NearZ * tanf(0.5f * m_FovY);
		m_FarWindowHeight = 2.0f * m_FarZ * tanf(0.5f * m_FovY);

		//dx���� �����ϴ� projection�� ���ϴ� �Լ�.
		Matrix P = XMMatrixPerspectiveFovLH(m_FovY, m_Aspect, m_NearZ, m_FarZ);
		XMStoreFloat4x4(&m_Proj, P);

		//Frustum* _Frustum = m_GameObject->GetComponent<Frustum>();
		if (m_Frustum)
			m_Frustum->OnResize();

	}

	void Camera::LookAt(Vector3 _target, Vector3 _pos)
	{
		m_Transform->LookAt(_target, _pos);
	}

	void Camera::Pitch(float angle)
	{
		// Right���͸� �������� up look ȸ��
		m_Transform->Pitch(angle);
	}

	void Camera::RotateY(float angle)
	{
		// ���� Y�� �������� 3�� ȸ��
		m_Transform->RotateY(angle);
	}

	std::shared_ptr<Transform> Camera::GetTransform()
	{
		return m_Transform;
	}

}
