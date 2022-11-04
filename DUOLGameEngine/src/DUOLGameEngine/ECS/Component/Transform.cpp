#include "DUOLGameEngine/ECS/Component/Transform.h"

#include "DUOLGameEngine/ECS/GameObject.h"

namespace DUOLGameEngine
{
	Transform::Transform(const std::weak_ptr<DUOLGameEngine::GameObject>& owner) :
		ComponentBase(owner, DUOLCommon::StringHelper::ToTString("Transform"))
		, _parent(nullptr)
		, _children(std::vector<std::weak_ptr<Transform>>())
		, _localPosition(Vector3::Zero)
		, _localScale(Vector3::One)
		, _localRotation(Quaternion::Identity)
		, _localEulerAngle(Vector3::Zero)
		, _worldPosition(Vector3::Zero)
		, _worldScale(Vector3::One)
		, _worldRotation(Quaternion::Identity)
		, _worldEulerAngle(Vector3::Zero)
		, _localMatrix(Matrix::Identity)
		, _worldMatrix(Matrix::Identity)
	{

	}

	Transform::~Transform()
	{

	}

	void Transform::UpdateTM()
	{
		UpdateLocalTM();

		UpdateWorldTM();

		UpdateChildrenTM();
	}

	void Transform::UpdateLocalTM()
	{
		_localMatrix = Matrix::CreateScale(_localScale) * Matrix::CreateFromQuaternion(_localRotation) *
			Matrix::CreateTranslation(_localPosition);
	}

	void Transform::UpdateWorldTM()
	{
		if (_parent != nullptr)
		{
			const Matrix& parentWorldTM = _parent->GetWorldMatrix();

			_worldMatrix = parentWorldTM * _localMatrix;
		}
		else
		{
			_worldMatrix = _localMatrix;
		}
	}

	void Transform::UpdateChildrenTM() const
	{
		for (auto& child : _children)
		{
			if (child.lock() != nullptr)
			{
				/// <summary>
				/// �θ��� Ʈ�������� �ٲ�� �ڽ��� ���� ������Ƽ�� �ٲ��.
				/// </summary>
				child.lock()->GetTransform()->UpdateTMAndAllProperties();
			}
		}
	}

	void Transform::UpdateTMAndAllProperties()
	{
		UpdateLocalTM();

		UpdateWorldTM();

		UpdateChildrenTM();

		DecomposeLocalTM();

		DecomposeWorldTM();
	}

	const Matrix& Transform::GetLocalMatrix() const
	{
		return _localMatrix;
	}

	const Matrix& Transform::GetWorldMatrix() const
	{
		return _worldMatrix;
	}

	void Transform::LookAt(const std::shared_ptr<Transform>& target, const Vector3& worldUp)
	{
		LookAt(target->GetWorldPosition(), worldUp);
	}

	void Transform::LookAt(const Vector3& worldPosition, const Vector3& worldUp)
	{
		// ���ο� Look Vector
		Vector3 newLook;

		(worldPosition - _worldPosition).Normalize(newLook);

		// ���ο� Right Vector
		Vector3 newRight;

		newLook.Cross(worldUp, newRight);

		newRight.Normalize();

		// ���ο� Up Vector
		Vector3 newUp;

		newRight.Cross(newLook, newUp);

		newUp.Normalize();

		const Matrix newLocalRot = Matrix::CreateFromLookRightUp(newLook, newRight, newUp);

		const Quaternion newLocalRotQuat = Quaternion::CreateFromRotationMatrix(newLocalRot);

		UpdateRotation(newLocalRotQuat, Space::Self);
	}

	void Transform::Rotate(const Vector3& eulers, Space relativeTo)
	{
		const Vector3 radianEulers = Vector3(MathHelper::DegreeToRadian(eulers.x), MathHelper::DegreeToRadian(eulers.y),
			MathHelper::DegreeToRadian(eulers.z));

		const Quaternion quat = Quaternion::CreateFromYawPitchRoll(radianEulers.y, radianEulers.x, radianEulers.z);

		UpdateRotation(quat, relativeTo);
	}

	void Transform::Rotate(const Vector3& axis, float angle, Space relativeTo)
	{
		const float radian = MathHelper::DegreeToRadian(angle);

		const Quaternion quat =
			DUOLMath::Quaternion::CreateFromAxisAngle(axis, radian);

		UpdateRotation(quat, relativeTo);
	}

	void Transform::RotateAround(const Vector3& point, const Vector3& axis, float angle)
	{
		// Point�� �θ�� ���ϰ� �������� ��ȯ��Ų��.
		const Vector3 diff = (_worldPosition - point);

		const Matrix rotateAroundMat = Matrix::CreateFromAxisAngle(axis, angle) * Matrix::CreateTranslation(diff);

		const Matrix newWorldTM = rotateAroundMat * _worldMatrix;

		_worldMatrix = newWorldTM;

		if (_parent != nullptr)
			_localMatrix = _worldMatrix * (_parent->GetWorldMatrix().Invert());
		else
			_localMatrix = _worldMatrix;

		DecomposeLocalTM();

		DecomposeWorldTM(); 
	}

	void Transform::Translate(const Vector3& translation, Space relativeTo)
	{
		if (relativeTo == Space::Self)
		{
			const Matrix& localRot = Matrix::CreateFromQuaternion(_localRotation);

			const Vector3 movement = Vector3::Transform(translation, localRot);

			_worldPosition = _worldPosition + movement;
		}
		else
		{
			// DUOLGameEngine������ Scene�� World Coordinate�� �ٲٴ� ����� ������ ������ ���� �ٷ� ����մϴ�.
			_worldPosition = _worldPosition + translation;
		}

		if (_parent != nullptr)
		{
			const Quaternion& parentRot = _parent->GetWorldRotation();

			const Vector3& parentPos = _parent->GetWorldPosition();

			const Matrix parentWorldTM = Matrix::CreateFromQuaternion(parentRot) * Matrix::CreateTranslation(parentPos);

			Matrix parentWorldTMInv;

			parentWorldTM.Invert(parentWorldTMInv);

			// ���� ��ġ�� ���� ��ġ���� �θ� ��ȯ�� ����ȯ�� ���ϸ� �ȴ�. (������ ����)
			_localPosition = Vector3::Transform(_worldPosition, parentWorldTMInv);
		}
		else
		{
			_localPosition = _worldPosition;
		}

		UpdateTM();
	}

	void Transform::Translate(const Vector3& translation, const std::shared_ptr<Transform>& relativeTo)
	{
		if (relativeTo == nullptr)
		{
			Translate(translation, Space::World);

			return;
		}

		const Matrix& localRot = Matrix::CreateFromQuaternion(relativeTo->GetLocalRotation());

		const Vector3 movement = Vector3::Transform(translation, localRot);

		Translate(movement, Space::World);
	}

	void Transform::SetLocalScale(const Vector3& localScale)
	{
		_localScale = localScale;

		UpdateTM();
	}

	void Transform::UpdateRotation(const Quaternion& rotation, Space relativeTo)
	{
		if (relativeTo == Space::Self)
		{
			_localRotation = _localRotation * rotation;

			Quaternion parentRotation = Quaternion::Identity;

			if (_parent != nullptr)
				parentRotation = _parent->GetWorldRotation();

			const Quaternion newWorldRotation =  _localRotation * parentRotation;

			_worldRotation = newWorldRotation;
		}
		else
		{
			_worldRotation = _worldRotation * rotation;

			Quaternion parentRotationInv = Quaternion::Identity;

			if (_parent != nullptr)
				_parent->GetWorldRotation().Inverse(parentRotationInv);

			const Quaternion newLocalRotation = _worldRotation * parentRotationInv;

			_localRotation = newLocalRotation;
		}

		_localEulerAngle = Quaternion::ConvertQuaternionToEuler(_localRotation);

		_worldEulerAngle = Quaternion::ConvertQuaternionToEuler(_worldRotation);

		UpdateTM();

		UpdateLookRightUp();
	}

	void Transform::UpdateLookRightUp()
	{
		_look = Vector3::TransformNormal(Vector3::UnitZ, _worldMatrix);

		_right = Vector3::TransformNormal(Vector3::UnitX, _worldMatrix);

		_up = Vector3::TransformNormal(Vector3::UnitY, _worldMatrix);
	}

	void Transform::DecomposeLocalTM()
	{
		_localMatrix.Decompose(_localScale, _localRotation, _localPosition);

		_localEulerAngle = Quaternion::ConvertQuaternionToEuler(_localRotation);
	}

	void Transform::DecomposeWorldTM()
	{
		_worldMatrix.Decompose(_worldScale, _worldRotation, _worldPosition);

		_worldEulerAngle = Quaternion::ConvertQuaternionToEuler(_worldRotation);
	}

	std::shared_ptr<Transform> Transform::GetRootTransform()
	{
		std::shared_ptr<Transform> rootTransform = this->shared_from_this();

		// �� �̻��� �θ� ���� ������ ���� �ö󰩴ϴ�.
		while (rootTransform->_parent != nullptr)
		{
			rootTransform = rootTransform->_parent;
		}

		return rootTransform;
	}
	
	void Transform::SetParent(const std::shared_ptr<Transform>& parent, bool worldPositionStays)
	{
		Matrix parentWorldTM = Matrix::Identity;

		// ������ �θ� �����մϴ�.
		if (_parent != nullptr)
		{
			_parent->ResetChild(this->shared_from_this());
		}

		// �Ű����� parent�� �������ͷ� ���� ���
		if (parent == nullptr)
			_parent = nullptr;
		else
		{
			_parent = parent;

			_parent->_children.push_back(this->shared_from_this());

			parentWorldTM = _parent->GetWorldMatrix();
		}

		// ������ ���� ���� ������, ������, �����̼��� ������.
		if (worldPositionStays)
		{
			// ���带 �����ϱ� ���ؼ� ���� �ٲ� �θ� ����Ͽ� ������ �ٲ��.
			_localMatrix = _worldMatrix * parentWorldTM.Invert();

			DecomposeLocalTM();
		}
		// ������ ���� ���� ������, ������, �����̼��� ������.
		else
		{
			// ������ �����ϱ� ���ؼ� ���� �ٲ� �θ� ����Ͽ� ���尡 �ٲ��.
			_worldMatrix = _localMatrix * parentWorldTM;

			DecomposeWorldTM();

			// ���� ��Ʈ������ ����Ǿ����Ƿ� �ڽ� ������ ���� Ʈ������ ������Ʈ�� �ʿ��ϴ�.
			UpdateChildrenTM();
		}
	}

	std::shared_ptr<Transform> Transform::FindChild(const DUOLCommon::tstring& name) const
	{
		for (auto& child : _children)
		{
			std::shared_ptr<Transform> sharedChild = child.lock();

			if ((sharedChild != nullptr) && (sharedChild->GetGameObject()->GetName() == name))
				return sharedChild;
		}

		return nullptr;
	}

	std::shared_ptr<Transform> Transform::Find(const DUOLCommon::tstring& name) const
	{
		if (name.find('/') == false)
			return FindChild(name);

		std::shared_ptr<Transform> ret = nullptr;

		size_t prevIndex = 0;

		size_t index = name.find('/');

		while (index != DUOLCommon::tstring::npos)
		{
			DUOLCommon::tstring childName = DUOLCommon::tstring(name.begin() + prevIndex, name.begin() + index);

			ret = FindChild(childName);

			prevIndex = index;

			index = name.find('/', prevIndex);
		}

		return ret;
	}

	void Transform::DetachChildren()
	{
		for (auto& child : _children)
		{
			if (child.lock() != nullptr)
			{
				child.lock()->SetParent(nullptr, true);
			}
		}

		_children.clear();
	}

	bool Transform::IsChildOf(const std::shared_ptr<Transform>& parent) const
	{
		const std::vector<std::weak_ptr<Transform>> parentChildren = parent->GetChildren();

		for (const auto& child : parentChildren)
		{
			if ((child.lock() != nullptr) && (child.lock().get() == this))
			{
				// ����.
				return true;
			}
		}

		// ����.
		return false;
	}

	void Transform::ResetChild(const std::shared_ptr<Transform>& child)
	{
		// ����⸸ �ϸ� �˴ϴ�. �ڽ��� �������ٰ� �ؼ� �� �ڽĿ��� ���� ������ ���� ���� ..!
		std::erase_if(_children, [&child](const std::weak_ptr<DUOLGameEngine::Transform>& item)
			{
				// UUID�� ���ؾ��ϳ� ..?
				return (child == item.lock());
			});
	}
}