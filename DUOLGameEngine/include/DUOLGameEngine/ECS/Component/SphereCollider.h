/**

    @file      SphereCollider.h
    @brief     3D sphere-shaped primitive collider.
    @details   ~
    @author    COMETLEE
    @date      7.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/

#pragma once
#include "DUOLGameEngine/ECS/Component/ColliderBase.h"
#include "DUOLPhysics/Shapes/PhysicsSphere.h"

namespace DUOLGameEngine
{
	/**
	 * \brief 이것은 구형입니다.
	 */
	class SphereCollider final : public DUOLGameEngine::ColliderBase
	{
	public:
		SphereCollider(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name = TEXT("SphereCollider"));

		virtual ~SphereCollider() override;

	public:
		std::weak_ptr<DUOLPhysics::PhysicsSphere> _physicsSphere;

		DUOLMath::Vector3 _center;

		float _radius;

	public:
		virtual void OnEnable() override;

		virtual void OnDisable() override;

		inline const DUOLMath::Vector3& GetCenter() { return _center; }

		void SetCenter(const DUOLMath::Vector3& center);

		inline float GetRadius() { return _radius; }

		void SetRadius(float radius);

		friend class PhysicsManager;
	};
}