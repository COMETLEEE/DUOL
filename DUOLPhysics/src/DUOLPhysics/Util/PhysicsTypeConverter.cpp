#include "PhysicsTypeConverter.h"

namespace DUOLPhysics
{
	using namespace physx;

	PxCudaInteropMode::Enum ConvertInteropMode(RendererType rendererType)
	{
		switch (rendererType)
		{
		case RendererType::OPENGL: return PxCudaInteropMode::OGL_INTEROP;
		case RendererType::DIRECTX10: return PxCudaInteropMode::D3D10_INTEROP;
		case RendererType::DIRECTX11: return PxCudaInteropMode::D3D11_INTEROP;
		}

		return PxCudaInteropMode::NO_INTEROP;
	}

	PxU8 operator &(ShapeFlag flag, PxU8 mask)
	{
		return static_cast<PxU8>(flag) & mask;
	}

	PxShapeFlags ConvertShapeFlags(ShapeFlag flag)
	{
		PxShapeFlags retFlag;

		if ((flag & (1 << 0)) != 0)
			retFlag |= PxShapeFlag::Enum::eSIMULATION_SHAPE;

		if ((flag & (1 << 1)) != 0)
			retFlag |= PxShapeFlag::Enum::eSCENE_QUERY_SHAPE;

		if ((flag & (1 << 2)) != 0)
			retFlag |= PxShapeFlag::Enum::eTRIGGER_SHAPE;

		if ((flag & (1 << 3)) != 0)
			retFlag |= PxShapeFlag::Enum::eVISUALIZATION;

		return retFlag;
	}
}