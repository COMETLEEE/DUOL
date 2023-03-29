#include "Transform.h"
#include "../../DUOLMath/include/DUOLMath/DUOLMath.h"
#include "GameObject.h"

namespace Muscle
{
	Transform::Transform(std::shared_ptr<GameObject> _GameObject) :IComponents(_GameObject),
		m_Position(0.0f, 0.0f, 0.0f),
		m_Right(1.0f, 0.0f, 0.0f),
		m_Up(0.0f, 1.0f, 0.0f),
		m_Look(0.0f, 0.0f, 1.0f),
		m_Scale(1.0f, 1.0f, 1.0f),
		m_LocalTM(),
		m_WorldTM(),
		_isWorldTMUpdate(false)
	{
		MakeTM();
	}

	Transform::~Transform()
	{
	}

	DUOLMath::Vector3 Transform::GetPosition() const
	{
		return m_Position;
	}

	DUOLMath::Vector3 Transform::GetWorldPosition()
	{
		if (!_isWorldTMUpdate)
			GetWorldTM();
		return DUOLMath::Vector3(m_WorldTM._41, m_WorldTM._42, m_WorldTM._43);
	}

	void Transform::SetPosition(float x, float y, float z)
	{
		m_Position.x = x;
		m_Position.y = y;
		m_Position.z = z;
		MakeTM();
	}

	void Transform::SetPosition(const DUOLMath::Vector3& v)
	{
		m_Position = v;
		MakeTM();
	}

	void Transform::SetPosition(const DUOLMath::Vector4& v)
	{
		SetPosition(v.x, v.y, v.z);
	}

	void Transform::SetWorldPosition(const DUOLMath::Vector3& v)
	{
		std::shared_ptr<GameObject> parent = m_GameObject.lock()->GetParent();
		if (parent)
		{
			DUOLMath::Vector3 temp = v;

			temp = DUOLMath::Vector3::Transform(temp, parent->GetTransform()->GetWorldTM().Invert());
			SetPosition(temp);
		}
		else
			m_Position = v;

		MakeTM();
	}


	DUOLMath::Vector3 Transform::GetScale() const
	{
		return m_Scale;
	}

	DUOLMath::Vector3 Transform::GetXMScale() const
	{
		return XMLoadFloat3(&m_Scale);
	}

	DUOLMath::Vector3 Transform::GetEuler() const
	{
		return 	DUOLMath::Quaternion::ConvertQuaternionToEuler(m_Rotate);
	}

	void Transform::SetScale(float x, float y, float z)
	{
		m_Scale.x = x;
		m_Scale.y = y;
		m_Scale.z = z;
		MakeTM();
	}

	void Transform::SetScale(const DUOLMath::Vector3& v)
	{
		m_Scale = v;
		MakeTM();
	}

	DUOLMath::Vector3 Transform::GetRight() const
	{
		return m_Right;
	}

	DUOLMath::Vector3 Transform::GetUp() const
	{
		return m_Up;
	}

	DUOLMath::Vector3 Transform::GetWorldUp() const
	{
		return DUOLMath::Vector3(m_WorldTM._21, m_WorldTM._22, m_WorldTM._23);
	}

	DUOLMath::Vector3 Transform::GetLook() const
	{
		return m_Look;
	}

	DUOLMath::Vector3 Transform::GetXMRight() const
	{
		return XMLoadFloat3(&m_Right);
	}

	DUOLMath::Vector3 Transform::GetXMUp() const
	{
		return XMLoadFloat3(&m_Up);
	}

	DUOLMath::Vector3 Transform::GetXMLook() const
	{
		return XMLoadFloat3(&m_Look);
	}

	void Transform::SetLook(float x, float y, float z)
	{
		SetLook(DUOLMath::Vector3(x, y, z));
	}

	void Transform::SetLook(const DUOLMath::Vector3& pos)
	{
		/// <summary>
	/// target - Pos
	/// 월드 업 (외적) LookVector 
	/// LookVector (외적) RightVector 
	/// </summary>
		DUOLMath::Vector3 _floatUp(0.0f, 1.0f, 0.0f);
		DUOLMath::Vector3 _WorldUp = XMLoadFloat3(&_floatUp);

		DUOLMath::Vector3 Z = pos;
		DUOLMath::Vector3 X = DirectX::XMVector3Normalize(XMVector3Cross(_WorldUp, Z));
		DUOLMath::Vector3 Y = XMVector3Cross(Z, X);

		XMStoreFloat3(&m_Look, Z);
		XMStoreFloat3(&m_Right, X);
		XMStoreFloat3(&m_Up, Y);

		DUOLMath::Matrix _R_TM;
		_R_TM._11 = m_Right.x;	_R_TM._12 = m_Right.y;	_R_TM._13 = m_Right.z;	_R_TM._14 = 0;
		_R_TM._21 = m_Up.x;		_R_TM._22 = m_Up.y;		_R_TM._23 = m_Up.z;		_R_TM._24 = 0;
		_R_TM._31 = m_Look.x;	_R_TM._32 = m_Look.y;	_R_TM._33 = m_Look.z;	_R_TM._34 = 0;
		_R_TM._41 = 0;			_R_TM._42 = 0;			_R_TM._43 = 0;			_R_TM._44 = 1.0f;

		DirectX::XMVECTOR _Rotate, s, t; //st는 버리기 위함
		XMMatrixDecompose(&s, &_Rotate, &t, _R_TM);

		m_Rotate = _Rotate;
		MakeTM();
	}
	void Transform::SetLook(const DUOLMath::Vector4& pos)
	{
		SetLook(pos.x, pos.y, pos.z);
	}
	const DUOLMath::Vector4& Transform::GetQuaternion()
	{
		return m_Rotate;
	}
	void Transform::SetQuaternion(const DUOLMath::Vector4& _Quat)
	{
		//XMQuaternionMultiply()
		DUOLMath::Matrix _R = DUOLMath::Matrix::CreateFromQuaternion(_Quat);

		m_R_TM = _R;

		m_Right.x = 1;	m_Up.x = 0;	m_Look.x = 0;
		m_Right.y = 0;	m_Up.y = 1;	m_Look.y = 0;
		m_Right.z = 0;	m_Up.z = 0;	m_Look.z = 1;
		XMStoreFloat3(&m_Right, XMVector3TransformNormal(XMLoadFloat3(&m_Right), _R));
		XMStoreFloat3(&m_Up, XMVector3TransformNormal(XMLoadFloat3(&m_Up), _R));
		XMStoreFloat3(&m_Look, XMVector3TransformNormal(XMLoadFloat3(&m_Look), _R));

		m_Rotate = _Quat;

		MakeTM();

		//XMMATRIX SRT = m_S_MT * m_R_MT * m_T_MT;
		//SetXMLocalMT(SRT);
	}

	void Transform::SetEuler(const DUOLMath::Vector3& euler)
	{
		SetQuaternion(DUOLMath::Quaternion::CreateFromYawPitchRoll(euler.y, euler.x, euler.z));
	}

	void Transform::LookAt(const DUOLMath::Vector3& target, const DUOLMath::Vector3& pos)
	{
		/// <summary>
		/// target - Pos
		/// 월드 업 (외적) LookVector 
		/// LookVector (외적) RightVector 
		/// </summary>
		DUOLMath::Vector3 _floatUp(0.0f, 1.0f, 0.0f);
		DUOLMath::Vector3 _WorldUp = XMLoadFloat3(&_floatUp);

		DUOLMath::Vector3 Z = DirectX::XMVector3Normalize(XMVectorSubtract(target, pos));
		DUOLMath::Vector3 X = DirectX::XMVector3Normalize(XMVector3Cross(_WorldUp, Z));
		DUOLMath::Vector3 Y = XMVector3Cross(Z, X);

		XMStoreFloat3(&m_Position, pos);
		XMStoreFloat3(&m_Look, Z);
		XMStoreFloat3(&m_Right, X);
		XMStoreFloat3(&m_Up, Y);

		DUOLMath::Matrix _R_TM;
		_R_TM._11 = m_Right.x;	_R_TM._12 = m_Right.y;	_R_TM._13 = m_Right.z;	_R_TM._14 = 0;
		_R_TM._21 = m_Up.x;		_R_TM._22 = m_Up.y;		_R_TM._23 = m_Up.z;		_R_TM._24 = 0;
		_R_TM._31 = m_Look.x;	_R_TM._32 = m_Look.y;	_R_TM._33 = m_Look.z;	_R_TM._34 = 0;
		_R_TM._41 = 0;			_R_TM._42 = 0;			_R_TM._43 = 0;			_R_TM._44 = 1.0f;

		DirectX::XMVECTOR _Rotate, s, t; //st는 버리기 위함
		XMMatrixDecompose(&s, &_Rotate, &t, _R_TM);

		m_Rotate = _Rotate;

		MakeTM();
	}
	void Transform::LookAt(const DUOLMath::Vector3& target)
	{
		DUOLMath::Vector3 Pos = XMLoadFloat3(&m_Position);
		DUOLMath::Vector3 Tartget = XMLoadFloat3(&target);

		LookAt(Tartget, Pos);
		MakeTM();
	}

	void Transform::LookAt(std::shared_ptr<GameObject> target)
	{
		LookAt(target->GetTransform()->GetPosition());
	}

	void Transform::UpAt(const DUOLMath::Vector3& target, const DUOLMath::Vector3& pos)
	{
		DUOLMath::Vector3 _floatlook(0.0f, 0.0f, 1.0f);
		DUOLMath::Vector3 _WorldLook = XMLoadFloat3(&_floatlook);
		std::shared_ptr<GameObject> parent = m_GameObject.lock()->GetParent();
		DUOLMath::Vector3 lookTarget = target - pos;
		if (parent)
			lookTarget = DUOLMath::Vector3::Transform(target, parent->GetTransform()->GetWorldTM().Invert()) - pos;
		DUOLMath::Vector3 Y = DirectX::XMVector3Normalize(lookTarget);
		DUOLMath::Vector3 X = DirectX::XMVector3Normalize(XMVector3Cross(Y, _WorldLook));
		DUOLMath::Vector3 Z = DirectX::XMVector3Normalize(XMVector3Cross(X, Y));

		XMStoreFloat3(&m_Look, Z);
		XMStoreFloat3(&m_Right, X);
		XMStoreFloat3(&m_Up, Y);

		DUOLMath::Matrix _R_TM;
		_R_TM._11 = m_Right.x;	_R_TM._12 = m_Right.y;	_R_TM._13 = m_Right.z;	_R_TM._14 = 0;
		_R_TM._21 = m_Up.x;		_R_TM._22 = m_Up.y;		_R_TM._23 = m_Up.z;		_R_TM._24 = 0;
		_R_TM._31 = m_Look.x;	_R_TM._32 = m_Look.y;	_R_TM._33 = m_Look.z;	_R_TM._34 = 0;
		_R_TM._41 = 0;			_R_TM._42 = 0;			_R_TM._43 = 0;			_R_TM._44 = 1.0f;

		DirectX::XMVECTOR _Rotate, s, t; //st는 버리기 위함
		XMMatrixDecompose(&s, &_Rotate, &t, _R_TM);

		m_Rotate = _Rotate;

		MakeTM();


	}

	void Transform::UpAt(const DUOLMath::Vector3& target)
	{
		UpAt(target, GetPosition());
	}

	void Transform::UpAt(std::shared_ptr<GameObject> target)
	{
	}

	void Transform::Strafe(float d)
	{
		DUOLMath::Vector3 s = DirectX::XMVectorReplicate(d);
		DUOLMath::Vector3 r = XMLoadFloat3(&m_Right);
		DUOLMath::Vector3 p = XMLoadFloat3(&m_Position);
		XMStoreFloat3(&m_Position, XMVectorMultiplyAdd(s, r, p));
		MakeTM();
	}

	void Transform::Walk(float d)
	{
		DUOLMath::Vector3 s = DirectX::XMVectorReplicate(d);
		DUOLMath::Vector3 l = XMLoadFloat3(&m_Look);
		DUOLMath::Vector3 p = XMLoadFloat3(&m_Position);
		XMStoreFloat3(&m_Position, XMVectorMultiplyAdd(s, l, p));
		MakeTM();
	}

	void Transform::WorldUpDown(float d)
	{
		// mPosition += d * mUp
		DUOLMath::Vector3 scale = DirectX::XMVectorReplicate(d);
		DUOLMath::Vector3 up = XMLoadFloat3(&m_Up);
		DUOLMath::Vector3 position = XMLoadFloat3(&m_Position);
		XMStoreFloat3(&m_Position, XMVectorMultiplyAdd(scale, up, position));
		MakeTM();
	}

	void Transform::Pitch(float angle)
	{
		// Rotate up and look vector about the right vector.

		DUOLMath::Matrix R = DirectX::XMMatrixRotationAxis(XMLoadFloat3(&m_Right), angle);
		XMStoreFloat3(&m_Up, XMVector3TransformNormal(XMLoadFloat3(&m_Up), R));
		XMStoreFloat3(&m_Look, XMVector3TransformNormal(XMLoadFloat3(&m_Look), R));

		DUOLMath::Matrix _R_TM;
		_R_TM._11 = m_Right.x;	_R_TM._12 = m_Right.y;	_R_TM._13 = m_Right.z;	_R_TM._14 = 0;
		_R_TM._21 = m_Up.x;		_R_TM._22 = m_Up.y;		_R_TM._23 = m_Up.z;		_R_TM._24 = 0;
		_R_TM._31 = m_Look.x;	_R_TM._32 = m_Look.y;	_R_TM._33 = m_Look.z;	_R_TM._34 = 0;
		_R_TM._41 = 0;			_R_TM._42 = 0;			_R_TM._43 = 0;			_R_TM._44 = 1.0f;
		DirectX::XMVECTOR _Rotate, s, t; //st는 버리기 위함
		XMMatrixDecompose(&s, &_Rotate, &t, _R_TM);
		m_Rotate = _Rotate;

		MakeTM();
	}

	void Transform::RotateY(float angle)
	{
		// Rotate the basis vectors about the world y-axis.

		DUOLMath::Matrix R = DirectX::XMMatrixRotationY(angle);

		XMStoreFloat3(&m_Right, XMVector3TransformNormal(XMLoadFloat3(&m_Right), R));
		XMStoreFloat3(&m_Up, XMVector3TransformNormal(XMLoadFloat3(&m_Up), R));
		XMStoreFloat3(&m_Look, XMVector3TransformNormal(XMLoadFloat3(&m_Look), R));


		DUOLMath::Matrix _R_TM;
		_R_TM._11 = m_Right.x;	_R_TM._12 = m_Right.y;	_R_TM._13 = m_Right.z;	_R_TM._14 = 0;
		_R_TM._21 = m_Up.x;		_R_TM._22 = m_Up.y;		_R_TM._23 = m_Up.z;		_R_TM._24 = 0;
		_R_TM._31 = m_Look.x;	_R_TM._32 = m_Look.y;	_R_TM._33 = m_Look.z;	_R_TM._34 = 0;
		_R_TM._41 = 0;			_R_TM._42 = 0;			_R_TM._43 = 0;			_R_TM._44 = 1.0f;
		DirectX::XMVECTOR _Rotate, s, t; //st는 버리기 위함
		XMMatrixDecompose(&s, &_Rotate, &t, _R_TM);
		m_Rotate = _Rotate;
		MakeTM();
	}

	void Transform::Roll(float angle)
	{

		// Rotate up and look vector about the right vector.

		DUOLMath::Matrix R = DirectX::XMMatrixRotationAxis(XMLoadFloat3(&m_Up), angle);
		XMStoreFloat3(&m_Right, XMVector3TransformNormal(XMLoadFloat3(&m_Right), R));
		XMStoreFloat3(&m_Look, XMVector3TransformNormal(XMLoadFloat3(&m_Look), R));

		DUOLMath::Matrix _R_TM;
		_R_TM._11 = m_Right.x;	_R_TM._12 = m_Right.y;	_R_TM._13 = m_Right.z;	_R_TM._14 = 0;
		_R_TM._21 = m_Up.x;		_R_TM._22 = m_Up.y;		_R_TM._23 = m_Up.z;		_R_TM._24 = 0;
		_R_TM._31 = m_Look.x;	_R_TM._32 = m_Look.y;	_R_TM._33 = m_Look.z;	_R_TM._34 = 0;
		_R_TM._41 = 0;			_R_TM._42 = 0;			_R_TM._43 = 0;			_R_TM._44 = 1.0f;
		DirectX::XMVECTOR _Rotate, s, t; //st는 버리기 위함
		XMMatrixDecompose(&s, &_Rotate, &t, _R_TM);
		m_Rotate = _Rotate;

		MakeTM();
	}

	void Transform::SetXMLocalTM(DUOLMath::Matrix _M)
	{
		XMStoreFloat4x4(&m_LocalTM, _M);
		DirectX::XMVECTOR _Scale, _Transform, _Rotate;
		XMMatrixDecompose(&_Scale, &_Rotate, &_Transform, _M);

		DUOLMath::Matrix _R = DirectX::XMMatrixRotationQuaternion(_Rotate); // 쿼터니언 너무 어려워~

		XMStoreFloat3(&m_Position, _Transform);
		XMStoreFloat3(&m_Scale, _Scale);

		m_Rotate = _Rotate;
		auto temp = DUOLMath::Matrix::CreateFromQuaternion(m_Rotate);
		m_Right = temp.Right();
		m_Up = temp.Up();
		m_Look = temp.Forward();
		XMStoreFloat3(&m_Right, XMVector3TransformNormal(XMLoadFloat3(&m_Right), _R));
		XMStoreFloat3(&m_Up, XMVector3TransformNormal(XMLoadFloat3(&m_Up), _R));
		XMStoreFloat3(&m_Look, XMVector3TransformNormal(XMLoadFloat3(&m_Look), _R));
		MakeTM();
	}

	void Transform::SetLocalTM(DUOLMath::Matrix& _M)
	{

		SetXMLocalTM(XMLoadFloat4x4(&_M));
	}

	void Transform::SetXMWorldTM(DUOLMath::Matrix _M)
	{
		if (m_GameObject.lock()->GetParent())
		{
			// WorldTM * ParentWorldTM-1 //Local 구하기.. 맞는듯 O Todo : 인버스 함수 바꿔야함! 
			DUOLMath::Matrix _XMLocalT = _M * XMMatrixInverse(nullptr, m_GameObject.lock()->GetParent()->GetTransform()->GetXMWorldTM());
			SetXMLocalTM(_XMLocalT);
		}
		else
			SetXMLocalTM(_M);
	}

	DUOLMath::Matrix Transform::GetXMLocalTM()
	{
		return XMLoadFloat4x4(&m_LocalTM);
	}

	DUOLMath::Matrix Transform::GetLocalTM()
	{
		return m_LocalTM;
	}

	DUOLMath::Matrix Transform::GetXMWorldTM()
	{
		std::shared_ptr<GameObject> parent = m_GameObject.lock()->GetParent();
		if (_isWorldTMUpdate)
			return m_WorldTM;
		_isWorldTMUpdate = true;
		if (parent)
		{
			std::shared_ptr<Transform> _Parent = parent->GetTransform();
			m_WorldTM = GetXMLocalTM() * _Parent->GetXMWorldTM();
			m_WorldTM.Decompose(m_WorldScale, m_WorldRotate, m_WorldPosition);
			return m_WorldTM;
		}
		else
		{
			m_WorldTM = GetXMLocalTM();
			m_WorldTM.Decompose(m_WorldScale, m_WorldRotate, m_WorldPosition);
			return m_WorldTM;
		}
	}

	DUOLMath::Matrix Transform::GetWorldTM()
	{
		if (_isWorldTMUpdate)
			return m_WorldTM;
		else
			XMStoreFloat4x4(&m_WorldTM, GetXMWorldTM());

		return m_WorldTM;
	}

	void Transform::MakeTM()
	{
		//일단 최적화 무시!
		DUOLMath::Matrix _S_TM = DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);;

		DUOLMath::Matrix _T_TM = DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

		DUOLMath::Matrix _R_TM = DUOLMath::Matrix::CreateFromQuaternion(m_Rotate);

		m_T_TM = _T_TM;
		m_R_TM = XMLoadFloat4x4(&_R_TM);
		m_S_TM = _S_TM;

		DUOLMath::Matrix SRT = m_S_TM * m_R_TM * m_T_TM;
		XMStoreFloat4x4(&m_LocalTM, SRT);

		std::function<void(const std::vector<std::shared_ptr<GameObject>>&)> _setisupdate = [&](const std::vector<std::shared_ptr<GameObject>>& _Childrens)
		{
			for (auto& iter : _Childrens)
			{
				iter->GetTransform()->SetisTMUpdate(false);
				_setisupdate(iter->GetChildrens());
			}
		};
		;
		_setisupdate(m_GameObject.lock()->GetChildrens());

		_isWorldTMUpdate = false;

		GetWorldTM();
	}

	DUOLMath::Matrix Transform::GetWorldTranformTM()
	{
		if (!_isWorldTMUpdate)
			GetWorldTM();
		return  DUOLMath::Matrix::CreateTranslation(m_WorldPosition);
	}

	DUOLMath::Matrix Transform::GetWorldRotateTM()
	{
		if (!_isWorldTMUpdate)
			GetWorldTM();
		return DUOLMath::Matrix::CreateFromQuaternion(m_WorldRotate);
	}

	DUOLMath::Matrix Transform::GetWorldScaleTM()
	{
		if (!_isWorldTMUpdate)
			GetWorldTM();
		return DUOLMath::Matrix::CreateScale(m_WorldScale);
	}

	void Transform::Start()
	{
		_isWorldTMUpdate = false;
		GetWorldTM();
	}

	void Transform::Update()
	{
		if (!_isWorldTMUpdate)
			GetWorldTM();
	}

	void Transform::Render()
	{
	}
}