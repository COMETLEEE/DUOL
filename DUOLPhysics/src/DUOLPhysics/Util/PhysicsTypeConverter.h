#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include "DUOLPhysics/PhysicsDescriptions.h"
#include "PxPhysicsAPI.h"

namespace DUOLPhysics
{
	using namespace physx;

	PxCudaInteropMode::Enum ConvertInteropMode(RendererType rendererType);

	PxU8 operator &(ShapeFlag flag, PxU8 mask);

	PxShapeFlags ConvertShapeFlags(ShapeFlag flag);
}