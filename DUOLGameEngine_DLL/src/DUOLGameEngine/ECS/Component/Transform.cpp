#include "DUOLGameEngine/ECS/Component/Transform.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::Transform>("Transform")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("Position", &DUOLGameEngine::Transform::GetLocalPositionWithoutCheck, & DUOLGameEngine::Transform::SetLocalPosition)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float3)
	)
	.property("Rotation", &DUOLGameEngine::Transform::GetLocalEulerAngleWithoutCheck, &DUOLGameEngine::Transform::SetLocalEulerAngle)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float3)
	)
	.property("Scale", &DUOLGameEngine::Transform::GetLocalScaleWithoutCheck, &DUOLGameEngine::Transform::SetLocalScale)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float3)
	)
	.property("_localPosition", &DUOLGameEngine::Transform::GetLocalPositionWithoutCheck, &DUOLGameEngine::Transform::SetLocalPosition)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_localRotation", &DUOLGameEngine::Transform::GetLocalRotationWithoutCheck, &DUOLGameEngine::Transform::SetLocalRotation)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_localScale", &DUOLGameEngine::Transform::GetLocalScaleWithoutCheck, &DUOLGameEngine::Transform::SetLocalScale)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_children", &DUOLGameEngine::Transform::_children)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::SerializeByUUID, true)
		, metadata(DUOLCommon::MetaDataType::MappingType, DUOLCommon::MappingType::FileUUID)
	)
	.property("_parent", &DUOLGameEngine::Transform::_parent)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::SerializeByUUID, true)
		, metadata(DUOLCommon::MetaDataType::MappingType, DUOLCommon::MappingType::FileUUID)
	);
}

namespace DUOLGameEngine
{
	Transform::Transform() :
		ComponentBase(nullptr, TEXT("Transform"))
		, _parent(nullptr)
		, _children(std::vector<DUOLGameEngine::Transform*>())
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
		, _isDirtTransform(false)
	{
	}

	Transform::Transform(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		ComponentBase(owner, name)
		, _parent(nullptr)
		, _children(std::vector<DUOLGameEngine::Transform*>())
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
		, _isDirtTransform(false)
	{
		
	}

	Transform::~Transform()
	{

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

	void Transform::UpdateTMAndAllProperties()
	{
		UpdateLocalTM();

		UpdateWorldTM();

		UpdateLookRightUp();

		DecomposeLocalTM();

		DecomposeWorldTM();
	}

	void Transform::CheckIsDirtAndUpdate()
	{
		if (_isDirtTransform)
		{
			UpdateTMAndAllProperties();

			_isDirtTransform = false;
		}
	}

	void Transform::SetDirtTransform()
	{
		_isDirtTransform = true;

		for (auto& child : _children)
		{
			if (child != nullptr)
				child->SetDirtTransform();
		}
	}

	const Vector3& Transform::GetLocalEulerAngleWithoutCheck()
	{
		return Vector3(DUOLMath::MathHelper::RadianToDegree(_localEulerAngle.x), 
			DUOLMath::MathHelper::RadianToDegree(_localEulerAngle.y), DUOLMath::MathHelper::RadianToDegree(_localEulerAngle.z));
	}

	const Vector3& Transform::GetLocalEulerAngle()
	{
		CheckIsDirtAndUpdate();

		return Vector3(DUOLMath::MathHelper::RadianToDegree(_localEulerAngle.x), DUOLMath::MathHelper::RadianToDegree(_localEulerAngle.y), DUOLMath::MathHelper::RadianToDegree(_localEulerAngle.z));
	}

	const Matrix& Transform::GetLocalMatrix()
	{
		CheckIsDirtAndUpdate();

		return _localMatrix;
	}

	const Matrix& Transform::GetWorldMatrix()
	{
		CheckIsDirtAndUpdate();

		return _worldMatrix;
	}

	void Transform::LookAt(Transform* target, const Vector3& worldUp)
	{
		LookAt(target->GetWorldPosition(), worldUp);
	}

	void Transform::LookAt(const Vector3& worldPosition, const Vector3& worldUp)
	{
		// ���� ����
		if (worldPosition == _worldPosition)
			return;

		// DUOLMath::Matrix::CreateLookAt => XMMatrixLookAtLH(...)
		// : Builds a view matrix for a left-handed coordinate system using a camera position, an up direction, and a focal point.
		// ����, �� ����� ������� �ϸ� �ش� ī�޶��� ���� ����� ���´�. ���⼭ ȸ������ ���� �����Ű�� �ش� worldPosition�� �ٶ󺸴�
		// ȸ�� ���·� �ǵ��� �� �ֽ��ϴ�.
		DUOLMath::Matrix lookAtMatrix = DUOLMath::Matrix::CreateLookAt(_worldPosition, worldPosition, worldUp);

		// ȸ���� �����ֱ� ���ؼ� ȸ�� ���� ������.
		Quaternion lookAtQuat = Quaternion::CreateFromRotationMatrix(lookAtMatrix);

		lookAtQuat.Inverse(lookAtQuat);

		_worldRotation = Quaternion::Identity;

		// ȸ�� ���� ������Ʈ
		UpdateRotation(lookAtQuat, Space::World);
	}

	void Transform::Rotate(const Vector3& eulers, Space relativeTo)
	{
		CheckIsDirtAndUpdate();

		const Vector3 radianEulers = Vector3(MathHelper::DegreeToRadian(eulers.x), MathHelper::DegreeToRadian(eulers.y),
			MathHelper::DegreeToRadian(eulers.z));

		// ���� ��ǥ �ý��ۿ� ���ؼ� ȸ���Ѵٴ� ���� �ǹ��ϰ� �ʹ� ..
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
		CheckIsDirtAndUpdate();

		const float radian = MathHelper::DegreeToRadian(angle);

		Quaternion deltaRot = Quaternion::Identity;

		if (relativeTo == Space::World)
		{
			deltaRot = Quaternion::CreateFromAxisAngle(axis, radian);
		}
		else if (relativeTo == Space::Self)
		{
			// Local Coordinate�� �°� ����. ���� ȸ���� ���ؼ� axis�� �����ش�.
			const Vector3 localAxis = 
				Vector3::TransformNormal(axis, Matrix::CreateFromLookRightUp(_look, _right, _up));

			deltaRot = Quaternion::CreateFromAxisAngle(localAxis, radian);
		}

		UpdateRotation(deltaRot, relativeTo);
	}

	void Transform::Rotate(const Quaternion& quat, Space relativeTo)
	{
		CheckIsDirtAndUpdate();

		if (relativeTo == Space::Self)
		{
			
		}

		UpdateRotation(quat, relativeTo);
	}

	void Transform::RotateAround(const Vector3& point, const Vector3& axis, float angle)
	{
		CheckIsDirtAndUpdate();

		// Point�� �θ�� ���ϰ� �������� ��ȯ��Ų��.
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

		SetDirtTransform();
	}

	void Transform::Translate(const Vector3& translation, Space relativeTo)
	{
		CheckIsDirtAndUpdate();

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

		SetDirtTransform();
	}

	void Transform::Translate(const Vector3& translation, Transform* relativeTo)
	{
		CheckIsDirtAndUpdate();

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

#pragma region SCALED_EVENT
		_scaledEvent.Invoke(localScale);
#pragma endregion

		SetDirtTransform();

		// �������� �� Ư���ϱ� ������ .. ������Ƽ���� ���� �ٲ�����.
		UpdateTMAndAllProperties();

		SetDirtTransform();
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

		SetDirtTransform();
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

		SetDirtTransform();
	}

	void Transform::SetWorldTM(const Matrix& worldMatrix)
	{
		// World Property
		_worldMatrix = worldMatrix;

		DecomposeWorldTM();

		if (_parent != nullptr)
			_localMatrix = _worldMatrix * _parent->GetWorldMatrix().Invert();
		else
			_localMatrix = _worldMatrix;

		DecomposeLocalTM();

		SetDirtTransform();
	}

	void Transform::SetLocalTM(const Matrix& localMatrix)
	{
		// Local Property
		_localMatrix = localMatrix;

		DecomposeLocalTM();

		// World Property
		if (_parent != nullptr)
			_worldMatrix = _localMatrix * _parent->GetWorldMatrix();
		else
			_worldMatrix = _localMatrix;

		DecomposeWorldTM();

		SetDirtTransform();
	}

	void Transform::SetLocalTMWithoutDirt(const Matrix& localMatrix)
	{
		_localMatrix = localMatrix;

		DecomposeLocalTM();

		// World Property
		if (_parent != nullptr)
			_worldMatrix = _localMatrix * _parent->GetWorldMatrix();
		else
			_worldMatrix = _localMatrix;

		DecomposeWorldTM();
	}

	void Transform::SetLocalPosition(const DUOLMath::Vector3& position)
	{
		SetPosition(position, Space::Self);
	}

	void Transform::SetLocalEulerAngle(const Vector3& eulers)
	{
		const Vector3 radianEulers = Vector3(MathHelper::DegreeToRadian(eulers.x), MathHelper::DegreeToRadian(eulers.y),
			MathHelper::DegreeToRadian(eulers.z));

		Quaternion rot = Quaternion::Identity;

		rot = Quaternion::CreateFromYawPitchRoll(radianEulers.y, radianEulers.x, radianEulers.z);

		SetRotation(rot, Space::Self);
	}

	void Transform::SetLocalRotation(const Quaternion& quaternion)
	{
		SetRotation(quaternion, Space::Self);
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

		SetDirtTransform();
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

		// �� �̻��� �θ� ���� ������ ���� �ö󰩴ϴ�.
		while (rootTransform->_parent != nullptr)
		{
			rootTransform = rootTransform->_parent;
		}

		return rootTransform;
	}

	void Transform::SetParent(Transform* parent, bool worldPositionStays)
	{
		Matrix parentWorldTM = Matrix::Identity;

		// ������ �θ� �����ߴٸ� �θ� �����մϴ�.
		if (_parent != nullptr)
		{
			_parent->ResetChild(this);
		}
		// ������ �θ� �������� �ʾҽ��ϴ�. => Root Object�����Ƿ� �ش� ������Ʈ�� ���� ���� Root Object list���� �����ݴϴ�.
		else
		{
			DUOLGameEngine::GameObject* gameObject = GetGameObject();

			DUOLGameEngine::Scene* scene = gameObject->GetScene();

			if (scene != nullptr)
				scene->RemoveInRootObjectsList(gameObject);
		}

		// �Ű����� parent�� �������ͷ� ���� ��� => ���� Root Object�� �ǰڽ��ϴ�.
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
			_parent = parent;

			_parent->_children.push_back(this);

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

			SetDirtTransform();
		}
	}

	Transform* Transform::FindChild(const DUOLCommon::tstring& name) const
	{
		for (auto& child : _children)
		{
			if ((child != nullptr) && (child->GetGameObject()->GetName() == name))
				return child;
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
		return _children;
	}

	std::vector<DUOLGameEngine::GameObject*> Transform::GetChildGameObjects() const
	{
		std::vector<GameObject*> ret{};

		// 1�� �ڽ� ������Ʈ���� ��Ƽ� ��ȯ�մϴ�.
		for (const auto& child : _children)
		{
			if (child != nullptr)
			{
				ret.push_back(child->GetGameObject());
			}
		}

		return ret;
	}

	void Transform::GetChildGameObjectsRecursively(std::vector<DUOLGameEngine::GameObject*>& addOutput)
	{
		for (const auto& child : _children)
		{
			if (child != nullptr)
			{
				addOutput.push_back(child->GetGameObject());

				child->GetChildGameObjectsRecursively(addOutput);
			}
		}
	}

	void Transform::OnAwake()
	{
		ComponentBase::OnAwake();
	}

	std::vector<DUOLGameEngine::GameObject*> Transform::GetAllChildGameObjects() const
	{
		std::vector<GameObject*> ret{};

		for (const auto& child : _children)
		{
			if (child != nullptr)
			{
				ret.push_back(child->GetGameObject());

				child->GetChildGameObjectsRecursively(ret);
			}
		}

		return ret;
	}

	void Transform::DetachChildren()
	{
		for (auto& child : _children)
		{
			if (child != nullptr)
			{
				child->SetParent(nullptr, true);
			}
		}

		_children.clear();
	}

	bool Transform::IsChildOf(Transform* parent) const
	{
		const std::vector<Transform*>& parentChildren = parent->_children;

		for (const auto& child : parentChildren)
		{
			if ((child != nullptr) && (child == this))
			{
				// ����.
				return true;
			}
		}

		// ����.
		return false;
	}

	bool Transform::IsRootObject() const
	{
		return (_parent == nullptr);
	}

	void Transform::ResetChild(Transform* child)
	{
		// ����⸸ �ϸ� �˴ϴ�. �ڽ��� �������ٰ� �ؼ� �� �θ� ���� ���۵� ���� ���� ..!
		std::erase_if(_children, [&child](DUOLGameEngine::Transform* item)
			{
				// UUID�� ���ؾ��ϳ� ..?
				return (child == item);
			});
	}
}