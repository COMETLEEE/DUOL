#pragma once
#include "IComponents.h"
#include "../../DUOLMath/include/DUOLMath/DUOLMath.h"

namespace Muscle
{
	class Transform : public IComponents
	{
	public:
		Transform(std::shared_ptr<GameObject> _GameObject);
		virtual ~Transform();

	public:

		// Get/Set world transform position.
		DUOLMath::Vector3 GetPosition() const;
		DUOLMath::Vector3 GetWorldPosition();

		void SetPosition(float x, float y, float z);
		void SetPosition(const DUOLMath::Vector3& v);
		void SetPosition(const DUOLMath::Vector4& v);

		void SetWorldPosition(const DUOLMath::Vector3& v);


		DUOLMath::Vector3 GetScale() const;
		DUOLMath::Vector3 GetXMScale() const;
		DUOLMath::Vector3 GetEuler() const;

		void SetScale(float x, float y, float z);
		void SetScale(const DUOLMath::Vector3& v);

		// Get transform basis vectors.
		DUOLMath::Vector3 GetRight() const;
		DUOLMath::Vector3 GetUp() const;
		DUOLMath::Vector3 GetWorldUp() const;
		DUOLMath::Vector3 GetLook() const;

		DUOLMath::Vector3 GetXMRight() const;
		DUOLMath::Vector3 GetXMUp() const;
		DUOLMath::Vector3 GetXMLook() const;

		void SetLook(float x, float y, float z);
		void SetLook(const DUOLMath::Vector3& pos);
		void SetLook(const DUOLMath::Vector4& pos);

		const DUOLMath::Vector4& GetQuaternion();
		void SetQuaternion(const DUOLMath::Vector4& _Quat);
		void SetEuler(const DUOLMath::Vector3& euler);

		// Define transform space via LookAt parameters.
		void LookAt(const DUOLMath::Vector3& target, const DUOLMath::Vector3& pos);
		void LookAt(const DUOLMath::Vector3& target);
		void LookAt(std::shared_ptr<GameObject> target);

		void UpAt(const DUOLMath::Vector3& target, const DUOLMath::Vector3& pos);
		void UpAt(const DUOLMath::Vector3& target);
		void UpAt(std::shared_ptr<GameObject> target);

		// Strafe/Walk the camera a distance d.
		void Strafe(float d);
		void Walk(float d);
		void WorldUpDown(float d);

		// Rotate the transform.
		void Pitch(float angle);
		void RotateY(float angle);
		void Roll(float angle);



		void SetXMLocalTM(DUOLMath::Matrix _M);
		void SetLocalTM(DUOLMath::Matrix& _M);
		void SetXMWorldTM(DUOLMath::Matrix _M);

		DUOLMath::Matrix GetXMLocalTM();
		DUOLMath::Matrix GetLocalTM();

		DUOLMath::Matrix GetXMWorldTM();
		DUOLMath::Matrix GetWorldTM();

		void MakeTM();

		const DUOLMath::Matrix& GetTranformTM() { return m_T_TM; }
		const DUOLMath::Matrix& GetRotateTM() { return m_R_TM; }
		const DUOLMath::Matrix& GetScaleTM() { return m_S_TM; }

		const DUOLMath::Vector3& GetWorldTranform() { return m_WorldPosition; }
		const DUOLMath::Quaternion& GetWorldRotate() { return m_WorldRotate; }
		const DUOLMath::Vector3& GetWorldScale() { return m_WorldScale; }


		DUOLMath::Matrix GetWorldTranformTM();
		DUOLMath::Matrix GetWorldRotateTM();
		DUOLMath::Matrix GetWorldScaleTM();


		void SetisTMUpdate(bool isbool) { _isWorldTMUpdate = isbool; }
	private:
		bool _isWorldTMUpdate;

		// 월드 값 캐싱.
		DUOLMath::Vector3 m_WorldPosition; // T
		DUOLMath::Vector3 m_WorldScale; // S
		DUOLMath::Quaternion m_WorldRotate; // R

		// 행렬을 만들기 위한 변수들
		DUOLMath::Vector3 m_Position; // T
		DUOLMath::Vector3 m_Scale; // S
		DUOLMath::Vector4 m_Rotate; // R

		DUOLMath::Vector3 m_Right; //R
		DUOLMath::Vector3 m_Up;
		DUOLMath::Vector3 m_Look;

		//나중에 스케일 추가하자 S?
		DUOLMath::Matrix m_LocalTM;
		DUOLMath::Matrix m_WorldTM;

		DUOLMath::Matrix m_T_TM;
		DUOLMath::Matrix m_R_TM;
		DUOLMath::Matrix m_S_TM;

	public:
		virtual void Start();
		virtual void Update();
		virtual void Render();
	};
}