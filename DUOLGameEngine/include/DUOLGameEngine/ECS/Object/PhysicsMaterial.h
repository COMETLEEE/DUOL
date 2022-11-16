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
	class PhysicsMaterial final : public DUOLGameEngine::ObjectBase
	{
	public:
		PhysicsMaterial(const DUOLCommon::tstring& name = TEXT("PhysicsMaterial"));

		virtual ~PhysicsMaterial() override;

	private:
		/**
		 * \brief Physics engine에서 사용되는 Physics Material
		 */
		DUOLPhysics::PhysicsMaterial* _physicsMaterial;

	public:
		inline DUOLPhysics::PhysicsMaterial* GetPrimitivePhysicsMaterial() const  { return _physicsMaterial; }

		inline void SetPrimitivePhysicsMaterial(DUOLPhysics::PhysicsMaterial* physicsMaterial) { _physicsMaterial = physicsMaterial; }
	};
}