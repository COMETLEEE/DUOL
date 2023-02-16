/**

    @file      CapsuleCollider.h
    @brief     3D capsule-shaped primitive collider. 
    @details   ~
    @author    COMETLEE
    @date      22.11.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/ECS/Component/ColliderBase.h"
#include "DUOLPhysics/Shapes/PhysicsCapsule.h"

namespace DUOLGameEngine
{
	enum class CapsuleDirection
	{
		X = 0
		, Y = 1
		, Z = 2
	};

	/**
	 * \brief 이것은 캡슐입니다.
	 */
	class DUOL_GAMEENGINE_API CapsuleCollider final : public DUOLGameEngine::ColliderBase
	{
	public:
		CapsuleCollider(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("CapsuleCollider"));

		virtual ~CapsuleCollider() override;

	private:
		std::weak_ptr<DUOLPhysics::PhysicsCapsule> _physicsCapsule;

		DUOLMath::Vector3 _center;

		float _height;

		float _radius;

		CapsuleDirection _currentDirection;

	private:
		void SetCapsuleLocalPose();

	public:
		virtual void OnEnable() override;

		virtual void OnDisable() override;

		const DUOLMath::Vector3& GetCenter() const;

		void SetCenter(const DUOLMath::Vector3& center);

		float GetHeight() const;

		void SetHeight(float height);

		float GetRadius() const;

		void SetRadius(float radius);

		CapsuleDirection GetDirection() const;

		void SetDirection(DUOLGameEngine::CapsuleDirection direction);

		RTTR_ENABLE(DUOLGameEngine::ColliderBase)

	private:
		DUOLCommon::EventListenerID _onScaledEventListenerID;

#pragma region FRIEND_CLASS
		friend class PhysicsManager;
#pragma endregion
	};
}