/**

    @file    PhysicsActorBase.h
    @brief   
    @details -
    @author  JKim
    @date    31.10.2022

**/
#pragma once
/* Shape */
#include "../Shapes/PhysicsShapeBase.h"

/* etc */
#include "../PhysicsDescriptions.h"
#include "DUOLMath/DUOLMath.h"

#include <memory>

namespace DUOLPhysics
{
    struct PhysicsBoundingBox
    {
        DUOLMath::Vector3 _min;

        DUOLMath::Vector3 _max;
    };

	class PhysicsActorBase
	{

    private:
        DUOLMath::Matrix _worldTransform;

    public:
        const DUOLMath::Matrix& GetWorldTransform() { return _worldTransform; }

        void SetWorldTransform(const DUOLMath::Matrix& worldTransform) { _worldTransform = worldTransform; }

        virtual PhysicsBoundingBox GetBoundingBox(float inflation = 1.01f) = 0;

        virtual void AttachShape(std::weak_ptr<PhysicsShapeBase> shape) = 0;

        virtual void DetachShape(std::weak_ptr<PhysicsShapeBase> shape, bool isWakeOnLostTouch = true) = 0;

        virtual void Release() = 0;
	};
}