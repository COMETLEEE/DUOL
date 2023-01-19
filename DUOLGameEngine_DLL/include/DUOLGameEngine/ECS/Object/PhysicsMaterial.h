/**

    @file      PhysicsMaterial.h
    @brief     Physics material describes how to handle colliding objects.
    @details   ~
    @author    COMETLEE
    @date      14.11.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/ECS/ObjectBase.h"
#include "DUOLPhysics/Material/PhysicsMaterial.h"

namespace DUOLGameEngine
{
	/**
	 * \brief The material used by the collider.
	 */
	class DUOL_GAMEENGINE_API PhysicsMaterial final : public DUOLGameEngine::ObjectBase, public std::enable_shared_from_this<PhysicsMaterial>
	{
	public:
		PhysicsMaterial(const DUOLCommon::tstring& name = TEXT("PhysicsMaterial"));

		virtual ~PhysicsMaterial() override;

	private:
		/**
		 * \brief Physics engine에서 사용되는 Physics Material
		 */
		std::weak_ptr<DUOLPhysics::PhysicsMaterial> _physicsMaterial;

		/**
		 * \brief Primitive pointer of physics material.
		 */
		DUOLPhysics::PhysicsMaterial* _primitivePhysicsMaterial;

	public:
		inline const std::weak_ptr<DUOLPhysics::PhysicsMaterial>& GetPhysicsMaterial() { return _physicsMaterial; }

		void SetPhysicsMaterial(const std::weak_ptr<DUOLPhysics::PhysicsMaterial>& physicsMaterial);
	};
}