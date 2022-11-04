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

		// Set frustum(����ü).
		void SetLens();

		void LookAt(Vector3 _target, Vector3 _pos);


		// Rotate the camera.
		void Pitch(float angle);
		void RotateY(float angle);


		std::shared_ptr<Transform> GetTransform();

	private:
		//Ʈ������ ������Ʈ
		std::shared_ptr<Transform> m_Transform;
		// Cache frustum properties.
		float m_NearZ; // ����� ����ü ��� �Ÿ�
		float m_FarZ; // �� ����ü ��� �Ÿ�
		float m_Aspect; // ��Ⱦ�� 
		float m_FovY; // ����ü ���� �þ߰�
		float m_NearWindowHeight; // ����� ȭ�� ����
		float m_FarWindowHeight; // �� ȭ�� ����

		Matrix m_Proj;
		Matrix m_View;

		Frustum* m_Frustum;

		bool m_isMainCamera;
		bool m_isCulling;
	public:
		float GetNearZ() { return m_NearZ; } // ����� ����ü ��� �Ÿ�
		float GetFarZ() { return m_FarZ; } // �� ����ü ��� �Ÿ�
		float GetAspect() { return m_Aspect; } // ��Ⱦ�� 
		float GetFovY() { return m_FovY; } // ����ü ���� �þ߰�
		float GetNearWindowHeight() { return m_NearWindowHeight; } // ����� ȭ�� ����
		float GetFarWindowHeight() { return m_FarWindowHeight; } // �� ȭ�� ����

		void SetMainCamera(bool _isbool) { m_isMainCamera = _isbool; }
		void SetisCuling(bool _isbool) { m_isCulling = _isbool; }
		// ��Ŀ�� ī�޶� �ٿ�?
		std::shared_ptr<GameObject> Pick(int _x, int _y);
		std::shared_ptr<CameraInfo> _cameraInfo;

	public:

		virtual void Start();
		virtual void Update(); // ��� Object Udpate���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ������Ʈ ���ֱ�.
		virtual void LateUpdate(); // ��� Object Udpate���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ������Ʈ ���ֱ�.
		virtual void Render(); // ��� Object ���� ���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ����
	};


}