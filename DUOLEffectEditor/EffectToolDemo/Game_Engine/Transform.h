#pragma once
#include "IComponents.h"

namespace Muscle
{
	class Transform : public IComponents
	{
	public:
		Transform(std::shared_ptr<GameObject> _GameObject);
		virtual ~Transform();

	public:

		// Get/Set world transform position.
		Vector3 GetPosition() const;
		Vector3 GetWorldPosition() ;

		void SetPosition(float x, float y, float z);
		void SetPosition(const Vector3& v);

		void SetWorldPosition(const Vector3& v);


		Vector3 GetScale() const;
		Vector3 GetXMScale() const;

		void SetScale(float x, float y, float z);
		void SetScale(const Vector3& v);

		// Get transform basis vectors.
		Vector3 GetRight() const;
		Vector3 GetUp() const;
		Vector3 GetWorldUp() const;
		Vector3 GetLook() const;

		Vector3 GetXMRight() const;
		Vector3 GetXMUp() const;
		Vector3 GetXMLook() const;

		void SetLook(const Vector3& pos);

		void SetQuaternion(const Vector4& _Quat);

		// Define transform space via LookAt parameters.
		void LookAt(const Vector3& target, const Vector3& pos);
		void LookAt(const Vector3& target);
		void LookAt(std::shared_ptr<GameObject> target);

		void UpAt(const Vector3& target, const Vector3& pos);
		void UpAt(const Vector3& target);
		void UpAt(std::shared_ptr<GameObject> target);

		// Strafe/Walk the camera a distance d.
		void Strafe(float d);
		void Walk(float d);
		void WorldUpDown(float d);

		// Rotate the transform.
		void Pitch(float angle);
		void RotateY(float angle);
		void Roll(float angle);



		void SetXMLocalTM(Matrix _M);
		void SetLocalTM(Matrix& _M);
		void SetXMWorldTM(Matrix _M);

		Matrix GetXMLocalTM();
		Matrix GetLocalTM();

		Matrix GetXMWorldTM();
		Matrix GetWorldTM();

		void MakeTM();

		const Matrix& GetTranformTM() { return m_T_TM; }
		const Matrix& GetRotateTM() { return m_R_TM; }
		const Matrix& GetScaleTM() { return m_S_TM; }

		const Vector3& GetWorldTranform() { return m_WorldPosition; }
		const Quaternion& GetWorldRotate() { return m_WorldRotate; }
		const Vector3& GetWorldScale() { return m_WorldScale; }


		Matrix GetWorldTranformTM();
		Matrix GetWorldRotateTM();
		Matrix GetWorldScaleTM();


		void SetisTMUpdate(bool isbool) { _isWorldTMUpdate = isbool; }
	private:
		bool _isWorldTMUpdate;

		// 월드 값 캐싱.
		Vector3 m_WorldPosition; // T
		Vector3 m_WorldScale; // S
		Quaternion m_WorldRotate; // R


		// 행렬을 만들기 위한 변수들
		Vector3 m_Position; // T
		Vector3 m_Scale; // S
		Vector4 m_Rotate; // S

		Vector3 m_Right; //R
		Vector3 m_Up;
		Vector3 m_Look;

		//나중에 스케일 추가하자 S?
		Matrix m_LocalTM;
		Matrix m_WorldTM;

		Matrix m_T_TM;
		Matrix m_R_TM;
		Matrix m_S_TM;

	public:
		virtual void Start();
		virtual void Update();
		virtual void Render();
	};
}