/**

    @file    PhysicsShapeBase.h
    @brief   
    @details -
    @author  JKim
    @date    31.10.2022

**/
#pragma once
#include "../PhysicsDescriptions.h"

namespace DUOLPhysics
{
	class PhysicsSystem;

	class PhysicsShapeBase
	{
		friend PhysicsSystem;

	public:
		virtual void Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc) = 0;

		virtual void Release() = 0;
	};
}