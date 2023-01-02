#include "DUOLGameEngine/ECS/Component/Transform.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

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
		, _look(DUOLMath::Vector3::Forward)
		, _right(DUOLMath::Vector3::Right)
		, _up(DUOLMath::Vector3::Up)
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

			_worldMatrix =  _localMatrix * parentWorldTM;
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
			if (!child.expired())
			{
				/// <summary>
				/// 부모의 트랜스폼이 바뀌면 자식의 월드 프로퍼티가 바뀐다.
				/// </summary>
				child.lock()->GetTransform()->UpdateTMAndAllProperties();
			}
			else
			{
				// 제거 ..?
			}
		}
	}

	void Transform::UpdateTMAndAllProperties()
	{
		UpdateLocalTM();

		UpdateWorldTM();

		UpdateLookRightUp();

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

	void Transform::LookAt(Transform* target, const Vector3& worldUp)
	{
		LookAt(target->GetWorldPosition(), worldUp);
	}

	void Transform::LookAt(const Vector3& worldPosition, const Vector3& worldUp)
	{
		const DUOLMath::Matrix lookAtMatrix = DUOLMath::Matrix::CreateLookAt(_worldPosition, worldPosition, worldUp);

		const Quaternion lookAtQuat = Quaternion::CreateFromRotationMatrix(lookAtMatrix);

		_worldRotation = lookAtQuat;

		// UpdateRotation(Quaternion::Identity, Space::World);
		UpdateRotation(lookAtQuat, Space::World);

#pragma region LOOK_AT_GOO_VERSION
		//// 새로운 Look Vector
		//Vector3 newLook;

		//(worldPosition - _worldPosition).Normalize(newLook);

		//// 새로운 Right Vector
		//Vector3 newRight;

		//newLook.Cross(worldUp, newRight);

		//newRight.Normalize();

		//// 새로운 Up Vector
		//Vector3 newUp;

		//newRight.Cross(newLook, newUp);

		//newUp.Normalize();

		//const Matrix newLocalRot = Matrix::CreateFromLookRightUp(newLook, newRight, newUp);

		//const Quaternion newLocalRotQuat = Quaternion::CreateFromRotationMatrix(newLocalRot);

		//UpdateRotation(newLocalRotQuat, Space::Self);
#pragma endregion
	}

	void Transform::Rotate(const Vector3& eulers, Space relativeTo)
	{
		const Vector3 radianEulers = Vector3(MathHelper::DegreeToRadian(eulers.x), MathHelper::DegreeToRadian(eulers.y),
			MathHelper::DegreeToRadian(eulers.z));

		// 각각 좌표 시스템에 대해서 회전한다는 것을 의미하고 싶다 ..
		Quaternion deltaRot = Quaternion::Identity;

		if (relativeTo == Space::World)
			deltaRot = Quaternion::CreateFromYawPitchRoll(radianEulers.y, radianEulers.x, radianEulers.z);
		else if (relativeTo == Space::Self)
			deltaRot = Quaternion::CreateFromAxisAngle(_look, radianEulers.z) *
				Quaternion::CreateFromAxisAngle(_right, radianEulers.x) * Quaternion::CreateFromAxisAngle(_up, radianEulers.y);

		UpdateRotation(deltaRot, relativeTo);
	}

	void Transform::Rotate(const Vector3& axis, float angle, Space relativeTo)
	{
		const float radian = MathHelper::DegreeToRadian(angle);

		Quaternion deltaRot = Quaternion::Identity;

		if (relativeTo == Space::World)
		{
			deltaRot = Quaternion::CreateFromAxisAngle(axis, radian);
		}
		else if (relativeTo == Space::Self)
		{
			// Local Coordinate에 맞게 조절. 현재 회전에 대해서 axis를 돌려준다.
			const Vector3 localAxis = 
				Vector3::TransformNormal(axis, Matrix::CreateFromLookRightUp(_look, _right, _up));

			deltaRot = Quaternion::CreateFromAxisAngle(localAxis, radian);
		}

		UpdateRotation(deltaRot, relativeTo);
	}

	void Transform::RotateAround(const Vector3& point, const Vector3& axis, float angle)
	{
		// Point를 부모로 취하고 포지션을 변환시킨다.
		const Vector3 diff = (_worldPosition - point);

		// degree to radian.
		const float radian = MathHelper::DegreeToRadian(angle);

		const Matrix rotateAroundMat = Matrix::CreateTranslation(diff) * Matrix::CreateFromAxisAngle(axis, radian)
			* Matrix::CreateTranslation(Vector3::Transform(-diff, Matrix::CreateFromAxisAngle(axis, radian)));

		const Matrix newWorldTM = _worldMatrix * rotateAroundMat;

		_worldMatrix = newWorldTM;

		if (_parent != nullptr)
			_localMatrix = _worldMatrix * (_parent->GetWorldMatrix().Invert());
		else
			_localMatrix = _worldMatrix;

		DecomposeLocalTM();

		DecomposeWorldTM();

		UpdateLookRightUp();

		UpdateChildrenTM();
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
			// DUOLGameEngine에서는 Scene의 World Coordinate를 바꾸는 기능이 없으니 다음과 같이 바로 계산합니다.
			_worldPosition = _worldPosition + translation;
		}

		if (_parent != nullptr)
		{
			const Quaternion& parentRot = _parent->GetWorldRotation();

			const Vector3& parentPos = _parent->GetWorldPosition();

			const Matrix parentWorldTM = Matrix::CreateFromQuaternion(parentRot) * Matrix::CreateTranslation(parentPos);

			Matrix parentWorldTMInv;

			parentWorldTM.Invert(parentWorldTMInv);

			// 로컬 위치는 월드 위치에서 부모 변환의 역변환을 취하면 된다. (스케일 제외)
			_localPosition = Vector3::Transform(_worldPosition, parentWorldTMInv);
		}
		else
		{
			_localPosition = _worldPosition;
		}

		UpdateTM();
	}

	void Transform::Translate(const Vector3& translation, Transform* relativeTo)
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

	void Transform::SetPosition(const Vector3& position, Space relativeTo)
	{
		if (relativeTo == Space::World)
		{
			_worldPosition = position;

			Vector3 parentPosition = Vector3::Zero;

			if (_parent != nullptr)
				parentPosition = _parent->GetTransform()->GetWorldPosition();

			_localPosition = position - parentPosition;
		}
		else
		{
			_localPosition = position;

			Vector3 parentPosition = Vector3::Zero;

			if (_parent != nullptr)
				parentPosition = _parent->GetTransform()->GetWorldPosition();

			_worldPosition = parentPosition + position;
		}

		UpdateTM();
	}

	void Transform::SetRotation(const Quaternion& rotation, Space relativeTo)
	{
		if (relativeTo == Space::World)
		{
			_worldRotation = rotation;

			Quaternion parentRotationInv = Quaternion::Identity;

			if (_parent != nullptr)
				_parent->GetWorldRotation().Inverse(parentRotationInv);

			const Quaternion newLocalRotation = _worldRotation * parentRotationInv;

			_localRotation = newLocalRotation;
		}
		else
		{
			_localRotation = rotation;

			Quaternion parentRotation = Quaternion::Identity;

			if (_parent != nullptr)
				parentRotation = _parent->GetWorldRotation();

			const Quaternion newWorldRotation = _localRotation * parentRotation;

			_worldRotation = newWorldRotation;
		}

		_localEulerAngle = Quaternion::ConvertQuaternionToEuler(_localRotation);

		_worldEulerAngle = Quaternion::ConvertQuaternionToEuler(_worldRotation);

		UpdateTM();

		UpdateLookRightUp();
	}

	void Transform::SetWorldTM(const Matrix& worldMatrix)
	{
		// World Property
		_worldMatrix = worldMatrix;

		_worldMatrix.Decompose(_worldScale, _worldRotation, _worldPosition);

		_worldEulerAngle = Quaternion::ConvertQuaternionToEuler(_worldRotation);

		// Local Property
		if (_parent != nullptr)
			_localMatrix = _worldMatrix * _parent->GetWorldMatrix().Invert();
		else
			_localMatrix = _worldMatrix;

		_localMatrix.Decompose(_localScale, _localRotation, _localPosition);

		_localEulerAngle = Quaternion::ConvertQuaternionToEuler(_localRotation);

		UpdateChildrenTM();

		UpdateLookRightUp();
	}

	void Transform::SetLocalTM(const Matrix& localMatrix)
	{
		// Local Property
		_localMatrix = localMatrix;

		_localMatrix.Decompose(_localScale, _localRotation, _localPosition);

		_localEulerAngle = Quaternion::ConvertQuaternionToEuler(_localRotation);

		// World Property
		if (_parent != nullptr)
			_worldMatrix = _localMatrix * _parent->GetWorldMatrix();
		else
			_worldMatrix = _localMatrix;

		_worldMatrix.Decompose(_worldScale, _worldRotation, _worldPosition);

		_worldEulerAngle = Quaternion::ConvertQuaternionToEuler(_worldRotation);

		UpdateChildrenTM();

		UpdateLookRightUp();
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

	Transform* Transform::GetRootTransform()
	{
		Transform* rootTransform = this;

		// 더 이상의 부모가 없을 때까지 위로 올라갑니다.
		while (rootTransform->_parent != nullptr)
		{
			rootTransform = rootTransform->_parent.get();
		}

		return rootTransform;
	}
	
	void Transform::SetParent(Transform* parent, bool worldPositionStays)
	{
		Matrix parentWorldTM = Matrix::Identity;

		// 기존의 부모가 존재했다면 부모를 정리합니다.
		if (_parent != nullptr)
		{
			_parent->ResetChild(this);
		}
		// 기존의 부모가 존재하지 않았습니다. => Root Object였으므로 해당 오브젝트가 속한 씬의 Root Object list에서 지워줍니다.
		else
		{
			DUOLGameEngine::GameObject* gameObject = GetGameObject();

			DUOLGameEngine::Scene* scene = gameObject->GetScene();

			if (scene != nullptr)
				scene->RemoveInRootObjectsList(gameObject);
		}

		// 매개변수 parent를 널포인터로 받은 경우 => 이제 Root Object가 되겠습니다.
		if (parent == nullptr)
		{
			_parent = nullptr;

			DUOLGameEngine::GameObject* gameObject = GetGameObject();

			DUOLGameEngine::Scene* scene = gameObject->GetScene();

			if (scene != nullptr)
				scene->AddInRootObjectsList(gameObject);
		}
		else
		{
			_parent = parent->shared_from_this();

			_parent->_children.push_back(this->shared_from_this());

			parentWorldTM = _parent->GetWorldMatrix();
		}

		// 이전과 같은 월드 포지션, 스케일, 로테이션을 가진다.
		if (worldPositionStays)
		{
			// 월드를 유지하기 위해서 새로 바뀐 부모에 비례하여 로컬이 바뀐다.
			_localMatrix = _worldMatrix * parentWorldTM.Invert();

			DecomposeLocalTM();
		}
		// 이전과 같은 로컬 포지션, 스케일, 로테이션을 가진다.
		else
		{
			// 로컬을 유지하기 위해서 새로 바뀐 부모에 비례하여 월드가 바뀐다.
			_worldMatrix = _localMatrix * parentWorldTM;

			DecomposeWorldTM();

			// 월드 매트릭스가 변경되었으므로 자식 계층에 대한 트랜스폼 업데이트가 필요하다.
			UpdateChildrenTM();
		}
	}

	Transform* Transform::FindChild(const DUOLCommon::tstring& name) const
	{
		for (auto& child : _children)
		{
			std::shared_ptr<Transform> sharedChild = child.lock();

			if ((sharedChild != nullptr) && (sharedChild->GetGameObject()->GetName() == name))
				return sharedChild.get();
		}

		return nullptr;
	}

	Transform* Transform::Find(const DUOLCommon::tstring& name) const
	{
		if (name.find('/') == false)
			return FindChild(name);

		Transform* ret = nullptr;

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

	std::vector<Transform*> Transform::GetChildren() const
	{
		std::vector<Transform*> ret;

		for (const auto& child : _children)
		{
			if (!child.expired())
				ret.push_back(child.lock().get());
		}

		return ret;
	}

	std::vector<DUOLGameEngine::GameObject*> Transform::GetChildGameObjects() const
	{
		std::vector<GameObject*> ret{};

		// 1차 자식 오브젝트들을 담아서 반환합니다.
		for (const auto& child : _children)
		{
			if (!child.expired())
				ret.push_back(child.lock()->GetGameObject());
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

	bool Transform::IsChildOf(Transform* parent) const
	{
		const std::vector<std::weak_ptr<Transform>>& parentChildren = parent->_children;

		for (const auto& child : parentChildren)
		{
			if ((child.lock() != nullptr) && (child.lock().get() == this))
			{
				// 있음.
				return true;
			}
		}

		// 없음.
		return false;
	}

	bool Transform::IsRootObject() const
	{
		return (_parent == nullptr);
	}

	void Transform::ResetChild(Transform* child)
	{
		// 지우기만 하면 됩니다. 자식이 없어진다고 해서 뭐 부모가 따로 조작될 것은 없음 ..!
		std::erase_if(_children, [&child](const std::weak_ptr<DUOLGameEngine::Transform>& item)
			{
				// UUID로 비교해야하나 ..?
				return (child == item.lock().get());
			});
	}
}