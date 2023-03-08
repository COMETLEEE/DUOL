/**

	@file      Transform.h
	@brief     Position, rotation and scale of an object.
	@details   ~
	@author    COMETLEE
	@date      20.10.2022
	@copyright © COMETLEE, 2022. All right reserved.

**/

#pragma once
#include "DUOLGameEngine/ECS/Component/ComponentBase.h"
#include "DUOLMath/DUOLMath.h"
#include "rttr/registration_friend.h"

// 게임 오브젝트 헤더 추가 금지

namespace DUOLGameEngine
{
	using namespace DUOLMath;

	// TODO 1
	// 일단 무지성 모든 각각의 조작에 대해서 행렬을 다시 계산하고
	// Decompose를 통해 업데이트된 포지션, 회전, 스케일 등을 다시 기록할 수 있도록 코드를 작성.
	// 최적화를 할 수 있는 방향 (행렬 업데이트 최소화, 디컴포즈 최소화 등 ..) 을 고려해서 업그레이드 해야한다.

	// TODO 2
	// 각각의 Translation matrix, rotation matrix, scale matrix 따로 캐싱하지 않고 함수에서 필요할 때마다
	// 조립해서 계산 중 .. 이것도 막으면 성능 향상 ..?

	/**
	 * \brief The Coordinate space in which to operate.
	 */
	enum class Space
	{
		/**
		 * \brief Use to transform a GameObject using DUOL's world coordinates, ignoring the GameObject's rotation state.
		 */
		World,

		/**
		 * \brief Use to transform a GameObject using its own coordinates and consider its rotation.
		 */
		 Self
	};

	/**
	 * \brief 게임 오브젝트의 공간 정보 (위치, 회전, 크기)를 담당하는 컴포넌트
	 * 모든 씬 내의 게임 오브젝트는 Transform을 가집니다.
	 */
	class DUOL_GAMEENGINE_API Transform final : public DUOLGameEngine::ComponentBase, public std::enable_shared_from_this<Transform>
	{
	public:
		Transform();

		Transform(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = TEXT("Transform"));

		virtual ~Transform();

	private:
		/**
		 * \brief The transform of the parent.
		 */
		DUOLGameEngine::Transform* _parent;

		/**
		 * \brief The list of the child transforms.
		 */
		std::vector<DUOLGameEngine::Transform*> _children;

	private:
		/**
		 * \brief Position of the transform relative to the parent transform.
		 */
		Vector3 _localPosition;

		/**
		 * \brief Scale of the transform relative to the GameObjects parent.
		 */
		Vector3 _localScale;

		/**
		 * \brief Rotation of the transform relative to the transform rotation of the parent.
		 */
		Quaternion _localRotation;

		/**
		 * \brief Rotation as euler angles in degrees relative to the parent transform's rotation.
		 */
		Vector3 _localEulerAngle;

		/**
		 * \brief World space position of the transform.
		 */
		Vector3 _worldPosition;

		/**
		 * \brief Global scale of the transform. (Read Only)
		 */
		Vector3 _worldScale;

		/**
		 * \brief World space rotation of the transform.
		 */
		Quaternion _worldRotation;

		/**
		 * \brief Rotation as euler angles in degrees that world space rotation of the transform.
		 */
		Vector3 _worldEulerAngle;

		/**
		 * \brief normalized vector in world space representing the look of transform considering its rotation.
		 */
		Vector3 _look;

		/**
		 * \brief normalized vector in world space representing the right of transform considering its rotation.
		 */
		Vector3 _right;

		/**
		 * \brief normalized vector in world space representing the up of transform considering its rotation.
		 */
		Vector3 _up;

		/**
		 * \brief local transform matrix relative to the parent transform.
		 */
		Matrix _localMatrix;

		/**
		 * \brief matrix that transform origin to world.
		 */
		Matrix _worldMatrix;

		/**
		 * \brief is dirt transform any property.
		 */
		bool _isDirtTransform;

#pragma region TRANSFORMATION
	private:
		void UpdateLocalTM();

		void UpdateWorldTM();

		void UpdateTMAndAllProperties();

		void CheckIsDirtAndUpdate();

		void SetDirtTransform();

	private:
		const Vector3& GetLocalPositionWithoutCheck() { return _localPosition; }

		const Vector3& GetLocalScaleWithoutCheck() { return _localScale; }

		const Quaternion& GetLocalRotationWithoutCheck() { return _localRotation; }

		const Vector3& GetLocalEulerAngleWithoutCheck();

	public:
		inline const Vector3& GetLocalPosition() { CheckIsDirtAndUpdate(); return _localPosition; }

		inline const Vector3& GetLocalScale() { CheckIsDirtAndUpdate(); return _localScale; }

		inline const Quaternion& GetLocalRotation() { CheckIsDirtAndUpdate(); return _localRotation; }

		const Vector3& GetLocalEulerAngle();

		inline const Vector3& GetWorldPosition() { CheckIsDirtAndUpdate(); return _worldPosition; }

		inline const Vector3& GetWorldScale() { CheckIsDirtAndUpdate(); return _worldScale; }

		inline const Quaternion& GetWorldRotation() { CheckIsDirtAndUpdate(); return _worldRotation; }

		inline const Vector3& GetWorldEulerAngle() { CheckIsDirtAndUpdate(); return _worldEulerAngle; }

		inline const Vector3& GetLook() { CheckIsDirtAndUpdate(); return _look; }

		inline const Vector3& GetUp() { CheckIsDirtAndUpdate(); return _up; }

		inline const Vector3& GetRight() { CheckIsDirtAndUpdate(); return _right; }

		[[nodiscard]]
		const Matrix& GetLocalMatrix();

		[[nodiscard]]
		const Matrix& GetWorldMatrix();

	public:
		/**
		 * \brief Rotate the transform so the look vector points at target's current position;
		 * \param target Object to point towards.
		 */
		void LookAt(Transform* target, const Vector3& worldUp = Vector3::Up);

		/**
		 * \brief Rotate the transform so the look vector points at worldPosition;
		 * \param target Point to look at.
		 */
		void LookAt(const Vector3& worldPosition, const Vector3& worldUp = Vector3::Up);

		/**
		 * \brief Applies a rotation of eulerangle.z degrees around the z-axis, x, y in that order.
		 * \param eulers The rotation to apply in euler angles.
		 * \param relativeTo Determines wherther to rotate the GameObjects either locally to the GameObject or relative to the Scene in world space.
		 */
		void Rotate(const Vector3& eulers, Space relativeTo = Space::Self);

		/**
		 * \brief Rotates the object around the given axis by the number of degrees defined by the given angle.
		 * \param axis The axis to apply rotation to.
		 * \param angle The degrees of rotation to apply.
		 * \param relativeTo Determines wherther to rotate the GameObjects either locally to the GameObject or relative to the Scene in world space.
		 */
		void Rotate(const Vector3& axis, float angle, Space relativeTo = Space::Self);

		/**
		 * \brief Rotates the transform about axis passing through point in world coordinates by angle degree.
		 * this modified both the position and the rotation of the transform.
		 * \param point center point of rotation.
		 * \param axis through line of center point. basis axis of rotation.
		 * \param angle degree angle to rotation.
		 */
		void RotateAround(const Vector3& point, const Vector3& axis, float angle);

		/**
		 * \brief Moves the transform in the direction and distance of translation
		 * \param translation Move direction and distance
		 * \param relativeTo Determines whether to rotate the GameObjects either locally to the GameObject or relative to the Scene in world space.
		 */
		void Translate(const Vector3& translation, Space relativeTo = Space::Self);

		/**
		 * \brief Moves the transform in the direction and distance of translation by relativeTo's local coordinate system.
		 * \param translation Move direction and distance
		 * \param relativeTo that movement applied coordinate system. if relativeTo is nullptr then the movement applied relative to the world coordinate system.
		 */
		void Translate(const Vector3& translation, Transform* relativeTo);

		/**
		 * \brief Scaling the transform using localScale locally.
		 * \param localScale The scale to apply scaling to.
		 */
		void SetLocalScale(const Vector3& localScale);

		/**
		 * \brief Moves the transform at the position of relativeTo coordinate.
		 * \param position position of transform.
		 * \param relativeTo that position applied coordinate system.
		 */
		void SetPosition(const Vector3& position, Space relativeTo = Space::World);

		/**
		 * \brief Rotates the transform to the rotation of relativeTo coordinate.
		 * \param rotation rotation of transform.
		 * \param relativeTo that position applied coordinate system.
		 */
		void SetRotation(const Quaternion& rotation, Space relativeTo = Space::World);

		/**
		 * \brief Set world matrix.
		 * \param worldMatrix matrix for update.
		 */
		void SetWorldTM(const Matrix& worldMatrix);

		/**
		 * \brief Set local matrix;
		 * \param localMatrix matrix for update.
		 */
		void SetLocalTM(const Matrix& localMatrix);

		/**
		 * \brief Set local matrix without dirt transform. (For Animator)
		 * \param localMatrix matrix for update.
		 */
		void SetLocalTMWithoutDirt(const Matrix& localMatrix);

		/**
		 * \brief Set local position and modify other properties.
		 * \param position The position to applying.
		 */
		void SetLocalPosition(const DUOLMath::Vector3& position);

		/**
		 * \brief Set local rotation by euler angle and modify other properties.
		 * \param eulers The angle to applying.
		 */
		void SetLocalEulerAngle(const Vector3& eulers);

		void SetLocalRotation(const Quaternion& quaternion);

	private:
		/**
		 * \brief Update Quaternion properties by relativeTo and Transform matrix.
		 * \param rotation Rotation Quaternion.
		 * \param relativeTo  Determines whether to rotate the GameObjects either locally to the GameObject or relative to the Scene in world space.
		 */
		void UpdateRotation(const Quaternion& rotation, Space relativeTo);

		/**
		 * \brief Update Look, Right, Up vector using world matrix.
		 */
		void UpdateLookRightUp();

		/**
		 * \brief Decompose local transform matrix and update all local properties.
		 */
		void DecomposeLocalTM();

		/**
		 * \brief Decompose world transform matrix and update all local properties.
		 */
		void DecomposeWorldTM();
#pragma endregion

#pragma region HIERARCHY
	public:
		/**
		 * \brief Find the root transform of this transform.
		 * \return The topmost transform in the hierarchy.
		 */
		Transform* GetRootTransform();

		/**
		 * \brief Get the parent transform.
		 * \return The parent transform.
		 */
		inline Transform* GetParent() const { return _parent; }

		/**
		 * \brief Set parent of the transform.
		 * \param parent The parent transform to using setting.
		 * \param worldPositionStays If true, the parent-relative position, scale and rotation are modified such that the object keeps the same world space position, rotation and scale as before.
		 */
		void SetParent(Transform* parent, bool worldPositionStays = true);

		[[nodiscard]]
		/**
		 * \brief Finds a child by name and returns it. does not descend the transform hierarchy.
		 * \param name Name of child to be found.
		 * \return child has name of parameter.
		 */
		Transform* FindChild(const DUOLCommon::tstring& name) const;

		[[nodiscard]]
		/**
		 * \brief Finds a child by name and returns it.
		 * \param name Name of child to be found. if name contains a '/' character it will access the transform
		 * it the hierarchy like a path name.
		 * \return child has name of parameter.
		 */
		Transform* Find(const DUOLCommon::tstring& name) const;

		[[nodiscard]]
		/**
		 * \brief Get children of the transform.
		 * \return the transforms of children.
		 */
		std::vector<Transform*> GetChildren() const;


		[[nodiscard]]
		/**
		 * \brief Get children objects of the transform.
		 * \return the gameobjects of children.
		 */
		std::vector<DUOLGameEngine::GameObject*> GetChildGameObjects() const;

		/**
		 * \brief Get children objects of the transform recursively.
		 * \return the child gameobjects of this.
		 */
		std::vector<DUOLGameEngine::GameObject*> GetAllChildGameObjects() const;

		/**
		 * \brief Unparents all children.
		 */
		void DetachChildren();

		/**
		 * \brief Is this transform a child of parent.
		 * \param parent Parent transform to be found.
		 * \return boolean value that indicates whether the transform is a child of a given transform.
		 */
		bool IsChildOf(Transform* parent) const;

		/**
		 * \brief Is this transform a root object's.
		 * \return boolean value that indicates whether this object is root object.
		 */
		bool IsRootObject() const;

	private:
		/**
		 * \brief Erase child in children list.
		 * \param child The child transform to reset in children list.
		 */
		void ResetChild(Transform* child);

		/**
		 * \brief Get all child game object recursively.
		 * \param addOutput push_back to this.
		 */
		void GetChildGameObjectsRecursively(std::vector<DUOLGameEngine::GameObject*>& addOutput);
#pragma endregion

	private:
		virtual void OnAwake() override;
		
		DUOLCommon::Event<void, const DUOLMath::Vector3&> _scaledEvent;

#pragma region FRIEND_CLASS
		friend class GameObject;

		friend class BoxCollider;

		friend class CapsuleCollider;

		friend class Animator;

		friend class PhysicsManager;

		RTTR_ENABLE(DUOLGameEngine::ComponentBase)

		RTTR_REGISTRATION_FRIEND
#pragma endregion
	};
}