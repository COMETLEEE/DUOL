#pragma once
#include "IComponents.h"
#include "../Library/inc/SimpleMath.h"

namespace Muscle
{
	class Transform : public IComponents
	{
	public:
		Transform(std::shared_ptr<GameObject> _GameObject);
		virtual ~Transform();

	public:

		// Get/Set world transform position.
		DirectX::SimpleMath::Vector3 GetPosition() const;
		DirectX::SimpleMath::Vector3 GetWorldPosition() ;

		void SetPosition(float x, float y, float z);
		void SetPosition(const DirectX::SimpleMath::Vector3& v);

		void SetWorldPosition(const DirectX::SimpleMath::Vector3& v);


		DirectX::SimpleMath::Vector3 GetScale() const;
		DirectX::SimpleMath::Vector3 GetXMScale() const;

		void SetScale(float x, float y, float z);
		void SetScale(const DirectX::SimpleMath::Vector3& v);

		// Get transform basis vectors.
		DirectX::SimpleMath::Vector3 GetRight() const;
		DirectX::SimpleMath::Vector3 GetUp() const;
		DirectX::SimpleMath::Vector3 GetWorldUp() const;
		DirectX::SimpleMath::Vector3 GetLook() const;

		DirectX::SimpleMath::Vector3 GetXMRight() const;
		DirectX::SimpleMath::Vector3 GetXMUp() const;
		DirectX::SimpleMath::Vector3 GetXMLook() const;

		void SetLook(const DirectX::SimpleMath::Vector3& pos);

		void SetQuaternion(const DirectX::SimpleMath::Vector4& _Quat);

		// Define transform space via LookAt parameters.
		void LookAt(const DirectX::SimpleMath::Vector3& target, const DirectX::SimpleMath::Vector3& pos);
		void LookAt(const DirectX::SimpleMath::Vector3& target);
		void LookAt(std::shared_ptr<GameObject> target);

		void UpAt(const DirectX::SimpleMath::Vector3& target, const DirectX::SimpleMath::Vector3& pos);
		void UpAt(const DirectX::SimpleMath::Vector3& target);
		void UpAt(std::shared_ptr<GameObject> target);

		// Strafe/Walk the camera a distance d.
		void Strafe(float d);
		void Walk(float d);
		void WorldUpDown(float d);

		// Rotate the transform.
		void Pitch(float angle);
		void RotateY(float angle);
		void Roll(float angle);



		void SetXMLocalTM(DirectX::SimpleMath::Matrix _M);
		void SetLocalTM(DirectX::SimpleMath::Matrix& _M);
		void SetXMWorldTM(DirectX::SimpleMath::Matrix _M);

		DirectX::SimpleMath::Matrix GetXMLocalTM();
		DirectX::SimpleMath::Matrix GetLocalTM();

		DirectX::SimpleMath::Matrix GetXMWorldTM();
		DirectX::SimpleMath::Matrix GetWorldTM();

		void MakeTM();

		const DirectX::SimpleMath::Matrix& GetTranformTM() { return m_T_TM; }
		const DirectX::SimpleMath::Matrix& GetRotateTM() { return m_R_TM; }
		const DirectX::SimpleMath::Matrix& GetScaleTM() { return m_S_TM; }

		const DirectX::SimpleMath::Vector3& GetWorldTranform() { return m_WorldPosition; }
		const DirectX::SimpleMath::Quaternion& GetWorldRotate() { return m_WorldRotate; }
		const DirectX::SimpleMath::Vector3& GetWorldScale() { return m_WorldScale; }


		DirectX::SimpleMath::Matrix GetWorldTranformTM();
		DirectX::SimpleMath::Matrix GetWorldRotateTM();
		DirectX::SimpleMath::Matrix GetWorldScaleTM();


		void SetisTMUpdate(bool isbool) { _isWorldTMUpdate = isbool; }
	private:
		bool _isWorldTMUpdate;

		// 월드 값 캐싱.
		DirectX::SimpleMath::Vector3 m_WorldPosition; // T
		DirectX::SimpleMath::Vector3 m_WorldScale; // S
		DirectX::SimpleMath::Quaternion m_WorldRotate; // R


		// 행렬을 만들기 위한 변수들
		DirectX::SimpleMath::Vector3 m_Position; // T
		DirectX::SimpleMath::Vector3 m_Scale; // S
		DirectX::SimpleMath::Vector4 m_Rotate; // S

		DirectX::SimpleMath::Vector3 m_Right; //R
		DirectX::SimpleMath::Vector3 m_Up;
		DirectX::SimpleMath::Vector3 m_Look;

		//나중에 스케일 추가하자 S?
		DirectX::SimpleMath::Matrix m_LocalTM;
		DirectX::SimpleMath::Matrix m_WorldTM;

		DirectX::SimpleMath::Matrix m_T_TM;
		DirectX::SimpleMath::Matrix m_R_TM;
		DirectX::SimpleMath::Matrix m_S_TM;

	public:
		virtual void Start();
		virtual void Update();
		virtual void Render();
	};
}