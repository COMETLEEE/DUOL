#pragma once
#include "IComponents.h"

namespace Muscle
{
	struct ::CameraInfo;

	class Transform;
	class Frustum;

	class Camera : public IComponents
	{
	public:
		Camera(std::shared_ptr<GameObject> _GameObj);
		virtual ~Camera();
	public:

		Matrix View();
		Matrix Proj();

		// Get frustum properties.
		float GetNearZ() const;
		float GetFarZ() const;
		float GetAspect() const;
		float GetFovY() const;
		float GetFovX() const;

		// Get near and far plane dimensions in view space coordinates.
		float GetNearWindowWidth() const;
		float GetNearWindowHeight() const;
		float GetFarWindowWidth() const;
		float GetFarWindowHeight() const;

		// Set frustum(절두체).
		void SetLens();

		void LookAt(Vector3 _target, Vector3 _pos);


		// Rotate the camera.
		void Pitch(float angle);
		void RotateY(float angle);


		std::shared_ptr<Transform> GetTransform();

	private:
		//트랜스폼 컴포넌트
		std::shared_ptr<Transform> m_Transform;
		// Cache frustum properties.
		float m_NearZ; // 가까운 절두체 평면 거리
		float m_FarZ; // 먼 절두체 평면 거리
		float m_Aspect; // 종횡비 
		float m_FovY; // 절두체 수직 시야각
		float m_NearWindowHeight; // 가까운 화면 높이
		float m_FarWindowHeight; // 먼 화면 높이

		Matrix m_Proj;
		Matrix m_View;

		Frustum* m_Frustum;

		bool m_isMainCamera;
		bool m_isCulling;
	public:
		float GetNearZ() { return m_NearZ; } // 가까운 절두체 평면 거리
		float GetFarZ() { return m_FarZ; } // 먼 절두체 평면 거리
		float GetAspect() { return m_Aspect; } // 종횡비 
		float GetFovY() { return m_FovY; } // 절두체 수직 시야각
		float GetNearWindowHeight() { return m_NearWindowHeight; } // 가까운 화면 높이
		float GetFarWindowHeight() { return m_FarWindowHeight; } // 먼 화면 높이

		void SetMainCamera(bool _isbool) { m_isMainCamera = _isbool; }
		void SetisCuling(bool _isbool) { m_isCulling = _isbool; }
		// 픽커를 카메라에 붙여?
		std::shared_ptr<GameObject> Pick(int _x, int _y);
		std::shared_ptr<CameraInfo> _cameraInfo;

	public:

		virtual void Start();
		virtual void Update(); // 모든 Object Udpate돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 업데이트 해주기.
		virtual void LateUpdate(); // 모든 Object Udpate돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 업데이트 해주기.
		virtual void Render(); // 모든 Object 렌더 돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 렌더
	};


}