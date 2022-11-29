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
	/**
	 * \brief 이것은 캡슐입니다.
	 */
	class CapsuleCollider final : public DUOLGameEngine::ColliderBase
	{
	public:
		CapsuleCollider(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name = TEXT("CapsuleCollider"));

		virtual ~CapsuleCollider() override;

	private:
		std::weak_ptr<DUOLPhysics::PhysicsCapsule> _physicsCapsule;

		DUOLMath::Vector3 _center;

		float _height;

		float _radius;

	public:
		virtual void OnEnable() override;

		virtual void OnDisable() override;

		inline const DUOLMath::Vector3& GetCenter() const { return _center; }

		void SetCenter(const DUOLMath::Vector3& center);

		inline float GetHeight() const { return _height; }

		void SetHeight(float height);

		inline float GetRadius() const { return _radius; }

		void SetRadius(float radius);

#pragma region FRIEND_CLASS
		friend class PhysicsManager;
#pragma endregion
	};
}